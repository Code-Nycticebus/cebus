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
    with open(doc, "a") as f:
        # This is what happens if you try to be clever
        doc_count: int = 0
        with open(src, "r") as d:
            for line in d.readlines():
                # If it encounters the comment terminator it makes 'doc_count' even
                if "*/" in line:
                    doc_count += 1

                # If 'doc_count' is odd it's in write mode
                elif doc_count % 2 == 1:
                    f.write(line)

                # If it encounters the 'DOCUMENTATION' symbol it makes 'doc_count' even
                elif "/* DOCUMENTATION" in line:
                    # If it's the first time it writes title and stuff to README
                    if doc_count == 0:
                        f.write(
                            f"## [{file.name}]({remote}/blob/main/{PurePosixPath(file.relative_to(cwd))})\n"
                        )
                    doc_count += 1


for file in src_dir.rglob("*.h"):
    doc = doc_dir / file.relative_to(src_dir).parent
    docs_dict[doc].append(file)

readme_file = doc_dir / "README.md"
with open(readme_file, "w") as f:
    f.truncate()

for docs, files in docs_dict.items():
    readme_file.write_text(f"# {docs.name.title()}\n")
    for file in files:
        write_src_to_doc(readme_file, file)
