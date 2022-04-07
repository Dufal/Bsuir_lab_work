develop a dirwalk program that scans the file system and outputs to
stdout information according to program options. The output format is similar
find output format.
dirwalk [dir] [options]
dir is the initial directory. If omitted, current (./).
options - options.
-l -- symlinks only (-type l)
-d -- directories only (-type d)
-f -- files only (-type f)
-s - sort output according to LC_COLLATE
If ldf options are omitted, directories, files, links are displayed, like find without
parameters.
