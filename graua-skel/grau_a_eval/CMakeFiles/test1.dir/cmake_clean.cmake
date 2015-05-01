FILE(REMOVE_RECURSE
  "CMakeFiles/test1.dir/test1.c.o"
  "test1.pdb"
  "test1"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang C)
  INCLUDE(CMakeFiles/test1.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
