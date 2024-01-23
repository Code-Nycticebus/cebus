from pathlib import Path, PurePosixPath
from collections import defaultdict
import subprocess
from typing import TextIO

remote = (
    subprocess.check_output(["git", "config", "--get", "remote.origin.url"])
    .decode()
    .strip()[:-4]
)

cwd = Path.cwd()
src_dir = Path(cwd, "src")
doc_dir = cwd / "docs"


docs_dict: dict[Path, list[Path]] = defaultdict(list)


def write_src_to_doc(doc: TextIO, src: Path):
    first = True
    with open(src, "r") as d:
        write_mode: bool = False
        for line in d.readlines():
            if "*/" in line:
                write_mode = False
            if write_mode:
                doc.write(line)
            if "/* DOCUMENTATION" in line:
                if first:
                    first = False
                    doc.write(
                        f"### [{file.name}]({remote}/blob/main/{PurePosixPath(file.relative_to(cwd))})\n"
                    )
                write_mode = True


# Collects the files
for file in src_dir.rglob("*.h"):
    doc = doc_dir / file.relative_to(src_dir).parent
    docs_dict[doc].append(file)

readme_file = doc_dir / "README.md"

# Removes any previous concatinated documentation
with open(readme_file, "r+") as f:
    for line in f.readlines():
        if "<!-- DOCUMENTATION -->" in line:
            f.truncate(f.tell())

# Iterates over all files and writes their content to readme
for docs, files in sorted(docs_dict.items()):
    with open(readme_file, "a") as f:
        f.write(f"## {docs.name.title()}\n")

        files.sort()
        for file in files:
            f.write(f" - [{file.name}](#{file.name.replace('.', '')})\n")
        for file in files:
            write_src_to_doc(f, file)
