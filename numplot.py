#!/usr/bin/env python3

"""
numplot.py

Reads:
    build/Release/output.txt

Writes:
    build/Release/nbody_trails_3d.png

Run from anywhere:
    python numplot.py

Matplotlib is installed automatically if it is missing.
"""

import sys
import subprocess
import re
from pathlib import Path


def ensure_package(import_name: str, pip_name: str | None = None):
    """Import a package, installing it with this Python interpreter if missing."""
    try:
        return __import__(import_name)
    except ImportError:
        package = pip_name or import_name
        print(f"{package} not found. Installing automatically...")

        subprocess.check_call([
            sys.executable,
            "-m",
            "pip",
            "install",
            package
        ])

        return __import__(import_name)


# Install/import matplotlib automatically.
ensure_package("matplotlib")
import matplotlib.pyplot as plt


STEP_RE = re.compile(r"^\s*Step\s+(\d+)\s*:\s*$")

BODY_RE = re.compile(
    r"^\s*Body\s+(\d+):\s*Position\(\s*"
    r"([+\-0-9.eE]+)\s*,\s*"
    r"([+\-0-9.eE]+)\s*,\s*"
    r"([+\-0-9.eE]+)\s*\)"
)


def find_release_dir() -> Path:
    """
    Find build/Release relative to:
      1. this script
      2. parent directories of this script
      3. current working directory
      4. parent directories of current working directory
    """

    candidates = []

    script_dir = Path(__file__).resolve().parent
    cwd = Path.cwd().resolve()

    for base in [script_dir, *script_dir.parents, cwd, *cwd.parents]:
        candidate = base / "build" / "Release"

        if candidate not in candidates:
            candidates.append(candidate)

    for candidate in candidates:
        if (candidate / "output.txt").is_file():
            return candidate

    searched = "\n".join(f"  {p / 'output.txt'}" for p in candidates)

    raise FileNotFoundError(
        "Could not find build/Release/output.txt.\n"
        "Searched:\n"
        f"{searched}"
    )


def parse_output(path: Path):
    """
    Returns:
        positions[body_id] = [(step, x, y, z), ...]
    """

    positions = {}
    current_step = None

    with path.open("r", encoding="utf-8", errors="replace") as file:
        for line in file:

            step_match = STEP_RE.match(line)

            if step_match:
                current_step = int(step_match.group(1))
                continue

            body_match = BODY_RE.match(line)

            if not body_match:
                continue

            body_id = int(body_match.group(1))
            x = float(body_match.group(2))
            y = float(body_match.group(3))
            z = float(body_match.group(4))

            # If the file has no Step lines for some reason,
            # fall back to sequential sample numbers.
            if current_step is None:
                current_step = len(positions.get(body_id, []))

            positions.setdefault(body_id, []).append(
                (current_step, x, y, z)
            )

    if not positions:
        raise RuntimeError(
            "No body positions were found in output.txt.\n"
            "Expected lines like:\n"
            "Body 0: Position(1.0, 2.0, 3.0), ..."
        )

    return positions


def set_equal_3d(ax, all_xyz):
    """
    Force equal physical scale on x/y/z so trajectories are not
    visually stretched by Matplotlib's default 3D aspect ratio.
    """

    xs = [p[0] for p in all_xyz]
    ys = [p[1] for p in all_xyz]
    zs = [p[2] for p in all_xyz]

    min_x, max_x = min(xs), max(xs)
    min_y, max_y = min(ys), max(ys)
    min_z, max_z = min(zs), max(zs)

    center_x = (min_x + max_x) / 2.0
    center_y = (min_y + max_y) / 2.0
    center_z = (min_z + max_z) / 2.0

    span = max(
        max_x - min_x,
        max_y - min_y,
        max_z - min_z
    )

    if span == 0.0:
        span = 1.0

    half = span / 2.0

    ax.set_xlim(center_x - half, center_x + half)
    ax.set_ylim(center_y - half, center_y + half)
    ax.set_zlim(center_z - half, center_z + half)

    ax.set_box_aspect((1, 1, 1))


def main():
    release_dir = find_release_dir()

    input_path = release_dir / "output.txt"
    output_path = release_dir / "nbody_trails_3d.png"

    print(f"Reading: {input_path}")

    positions = parse_output(input_path)

    body_ids = sorted(positions)

    sample_counts = [
        len(positions[body_id])
        for body_id in body_ids
    ]

    all_steps = [
        sample[0]
        for body_id in body_ids
        for sample in positions[body_id]
    ]

    print(f"Detected {len(body_ids)} bodies")
    print(
        f"Steps present: "
        f"{min(all_steps)} through {max(all_steps)}"
    )
    print(
        f"Samples per body: "
        f"{min(sample_counts)} to {max(sample_counts)}"
    )

    fig = plt.figure(figsize=(12, 10))
    ax = fig.add_subplot(111, projection="3d")

    all_xyz = []

    for body_id in body_ids:

        samples = positions[body_id]

        xs = [sample[1] for sample in samples]
        ys = [sample[2] for sample in samples]
        zs = [sample[3] for sample in samples]

        all_xyz.extend(zip(xs, ys, zs))

        # Full trail through every recorded simulation step.
        ax.plot(
            xs,
            ys,
            zs,
            linewidth=0.9,
            alpha=0.75
        )

        # Starting point.
        ax.scatter(
            xs[0],
            ys[0],
            zs[0],
            s=8,
            alpha=0.45
        )

        # Current/final point.
        ax.scatter(
            xs[-1],
            ys[-1],
            zs[-1],
            s=28
        )

    set_equal_3d(ax, all_xyz)

    ax.set_title(
        f"N-body simulation: {len(body_ids)} bodies, "
        f"{max(all_steps) - min(all_steps) + 1} recorded steps"
    )

    ax.set_xlabel("x (m)")
    ax.set_ylabel("y (m)")
    ax.set_zlabel("z (m)")

    fig.tight_layout()

    fig.savefig(
        output_path,
        dpi=200,
        bbox_inches="tight"
    )

    print(f"Saved: {output_path}")

    # Show an interactive Matplotlib window too.
    plt.show()


if __name__ == "__main__":
    try:
        main()
    except Exception as exc:
        print(f"\nERROR: {exc}", file=sys.stderr)
        sys.exit(1)
