from pathlib import Path, PurePosixPath
import subprocess


remote = (
    subprocess.check_output(["git", "config", "--get", "remote.origin.url"])
    .decode()
    .strip()[:-4]
)
cwd = Path.cwd()
src = Path(cwd, "src")

doc_files: list[Path] = []

for file in src.rglob("*.h"):
    docs = cwd / "docs" / file.relative_to(src).parent
    docs.mkdir(parents=True, exist_ok=True)
    with open(docs / "README.md", "w") as f:
        f.truncate()

for file in sorted(src.rglob("*.h")):
    docs = cwd / "docs" / file.relative_to(src).parent
    with open(docs / "README.md", "a") as f:
        f.write(f"# [{file.name}]({remote}/blob/main/{PurePosixPath(file.relative_to(cwd))})\n")

        writing = False
        with open(file, "r") as d:
            for line in d.readlines():
                if "*/" in line:
                    writing = False
                if writing:
                    f.write(line)
                if "/* DOCUMENTATION" in line:
                    writing = True
