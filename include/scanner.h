enum ScannerStringState
{
  SCAN_NUMBER,
  SCAN_LABEL,
  SCAN_IGNORE,
  SCAN_NONE,
  SCAN_UNDEFINED
};



char **scan_line(char *line);
char **scan_file(char *i_filename);
