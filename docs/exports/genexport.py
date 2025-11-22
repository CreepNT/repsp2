#!/usr/bin/env python3

# Generates basic Doxygen documentation page for a module's exports.
#
# Usage:
#  python ./genexport.py modname SceModName -l LibA:0xAAA,0xBBB -l LibB:0xCCC -o ./modname.dox
#
# where the -l arguments specify libraries exported by the module in the form 'NAME:NID[,NID]'.
#
# Generation is only done once; the generated page can be modified afterward.
# (Note that the module must also be manually added to `exports-root.dox`!)

import argparse
import sys

class LibraryInfo:
	def __init__(self, argument: str):
		def _ns_to_nid(nid_str):
			try:
				nid = int(nid_str, 16)
			except ValueError:
				raise argparse.ArgumentTypeError(f"NID must be hex, got '{nid_str}'")
			return nid

		if ':' not in argument:
			raise argparse.ArgumentTypeError(f"library '{argument}' must be NAME:NID[,NID], got '{argument}'")

		name, nids_string = argument.split(':', 1)

		self.name = name.strip()
		self.nids = [_ns_to_nid(n) for n in nids_string.split(',')]

		if len(self.nids) > 2:
			raise argparse.ArgumentTypeError(f"library {self.name} must not have more than two NIDs, got '{len(self.nids)}'")

	def __repr__(self):
		return f"LibraryInfo(name='{self.name}', nids={[hex(n) for n in self.nids]})"

	def __str__(self):
		return f"{self.name}:{','.join([hex(n) for n in self.nids])}"

def generate_export_file(group: str, modname: str, libraries: list[LibraryInfo]) -> str:
	def lib_page_tag(lib: LibraryInfo) -> str:
		return f'exportsLib_{lib.name}'

	module_exports_page_tag = f'exportsModule_{modname}'
	sorted_libs = sorted(libraries, key=lambda l: l.name.casefold())

	lines = []
	lines.append( "/**")
	lines.append(f"@page {module_exports_page_tag} {modname}")
	lines.append(f"@ref {group} \"{modname}\" exports the following libraries:")
	for lib in sorted_libs:
		lines.append(f"\t- @subpage {lib_page_tag(lib)}")
	lines.append("")


	for lib in sorted_libs:
		lines.append(f"@page {lib_page_tag(lib)} {lib.name}")
		lines.append( "<table>")
		lines.append(f"<caption>Library NID(s)</caption>")
		lines.append( "<tr><th>Firmware Version<th>NID")

		if len(lib.nids) == 1:
			lines.append(f"<tr><td>0.00~9.99<td>0x{lib.nids[0]:08X}")
		else:
			lines.append(f"<tr><td>0.00~3.61<td>0x{lib.nids[0]:08X}")
			lines.append(f"<tr><td>3.63~9.99<td>0x{lib.nids[1]:08X}")
		lines.append("</table>")
		lines.append("")
		lines.append("The following functions and/or variables are exported by this library:")
		lines.append("")

	lines.append("*/")
	return "\n".join(lines) + "\n"

def main(argv=None):
	p = argparse.ArgumentParser(description="Generate a doxygen exports comment block.")
	p.add_argument("group", help="group name corresponding to module (e.g., 'sysmem')")
	p.add_argument("modulename", help="module name (e.g., 'SceSysmem')")
	p.add_argument("-l", "--library", dest="libraries", action="append", type=LibraryInfo, required=True,
				   help="library exported by module, as NAME:NID[,NID]. Can be provided multiple times.")
	p.add_argument("-o", "--output", help="output file (default stdout)")
	args = p.parse_args(argv)

	exp_file = generate_export_file(args.group, args.modulename, args.libraries)

	if args.output:
		with open(args.output, "w", encoding="utf-8") as f:
			f.write(exp_file)
	else:
		sys.stdout.write(exp_file)

if __name__ == "__main__":
	main()
