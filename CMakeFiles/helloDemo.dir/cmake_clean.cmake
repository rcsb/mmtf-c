FILE(REMOVE_RECURSE
  "CMakeFiles/helloDemo.dir/src/demo.c.o"
  "helloDemo.pdb"
  "helloDemo"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang C)
  INCLUDE(CMakeFiles/helloDemo.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
