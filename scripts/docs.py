from pathlib import Path, PurePosixPath
from collections import defaultdict
from typing import TextIO

REMOTE = "https://github.com/Code-Nycticebus/cebus"

CWD = Path(__file__).parent.parent
SRC = Path(CWD, "src")
DOC = CWD
README = DOC / "README.md"

DOCUMENTATION_START = "/* DOCUMENTATION"
DOCUMENTATION_END = "*/"


def write_src_to_doc(doc: TextIO, src: Path):
    first = True
    with open(src, "r") as d:
        # This is kind of a state machine
        write_mode: bool = False
        for line in d.readlines():
            if line.startswith(DOCUMENTATION_END):
                write_mode = False
            if write_mode:
                doc.write(line)
            if line.startswith(DOCUMENTATION_START):
                if first:
                    first = False
                    doc.write(
                        f"# [{src.name}]({REMOTE}/blob/main/{PurePosixPath(src.relative_to(CWD))})\n"
                    )
                write_mode = True
    if not first:
        doc.write("\n")


def main() -> None:
    # Collects the files
    docs_dict: dict[Path, list[Path]] = defaultdict(list)
    for file in SRC.rglob("*.h"):
        doc = DOC / file.relative_to(SRC).parent
        docs_dict[doc].append(file)

    # Removes any doc.py created documentation
    with open(README, "r") as f:
        lines_to_keep = []
        for line in f.readlines():
            lines_to_keep.append(line)
            if "<!-- DOCUMENTATION -->" in line:
                break

    # Creates table of content
    with open(README, "w") as f:
        for line in lines_to_keep:
            f.write(line)

        sorted_entries = tuple(
            (docs, tuple(sorted(files))) for docs, files in sorted(docs_dict.items())
        )

        # Creates table of content
        for docs, files in sorted_entries:
            f.write(f"- [{docs.name.title()}](#{docs.name.title()})\n")
            for file in files:
                f.write(f"   - [{file.name}](#{file.name.replace('.', '')})\n")
        f.write("\n")

        # Writes documentation content
        for docs, files in sorted_entries:
            f.write(f"# {docs.name.title()}\n\n")

            for file in files:
                write_src_to_doc(f, file)


if __name__ == "__main__":
    main()
