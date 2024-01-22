from pathlib import Path, PurePosixPath
from collections import defaultdict
import subprocess


remote = (
    subprocess.check_output(["git", "config", "--get", "remote.origin.url"])
    .decode()
    .strip()[:-4]
)
cwd = Path.cwd()
src_dir = Path(cwd, "src")
doc_dir = cwd / "docs"


docs_dict: dict[Path, list[Path]] = defaultdict(list)


def write_src_to_doc(doc: Path, src: Path):
    first = True
    with open(doc, "a") as f:
        write_mode: bool = False
        with open(src, "r") as d:
            for nr, line in enumerate(d.readlines()):
                if "*/" in line:
                    write_mode = False

                if write_mode:
                    f.write(line)

                if "/* DOCUMENTATION" in line:
                    if first:
                        first = False
                        f.write(
                            f"### [{file.name}]({remote}/blob/main/{PurePosixPath(file.relative_to(cwd))}#L{nr+1})\n"
                        )
                    write_mode = True


for file in src_dir.rglob("*.[c|h]"):
    doc = doc_dir / file.relative_to(src_dir).parent
    docs_dict[doc].append(file)

readme_file = doc_dir / "README.md"
with open(readme_file, "w") as f:
    f.truncate()

for docs, files in docs_dict.items():
    with open(readme_file, "a") as f:
        f.write(f"## {docs.name.title()}\n")
    for file in files:
        write_src_to_doc(readme_file, file)
