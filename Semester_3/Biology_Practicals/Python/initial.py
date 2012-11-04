import pkg_resources
eggs = pkg_resources.require("TurboGears")
from setuptools.archive_util import unpack_archive
for egg in eggs:
   if os.path.isdir(egg.location):
       sys.path.insert(0, egg.location)
       continue
   unpack_archive(egg.location, eggdir)
eggpacks = set()
eggspth = open("build/eggs.pth", "w")
for egg in eggs:
   print egg
   eggspth.write(os.path.basename(egg.location))
   eggspth.write("\n")
   eggpacks.update(egg.get_metadata_lines("top_level.txt"))
eggspth.close()
eggpacks.remove("pkg_resources")

import zipfile
oldzipfile = "dist/exe/library.zip"
newzipfile = "dist/exe/small-library.zip"
oldzip = zipfile.ZipFile(oldzipfile, "r")
newzip = zipfile.ZipFile(newzipfile, "w", zipfile.ZIP_STORED)
for entry in oldzip.infolist():
   delim = entry.filename.find("/")
   if delim == -1:
       delim = entry.filename.find(".")
   if delim > -1:
       if entry.filename[0:delim] in eggpacks:
           print "Skipping %s, it's in the egg" % (entry.filename)
           continue
   newzip.writestr(entry, oldzip.read(entry.filename))
newzip.close()
oldzip.close()
os.remove(oldzipfile)
os.rename(newzipfile, oldzipfile)