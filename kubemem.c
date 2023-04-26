#include "stdlib.h"
#include "stdio.h"
#include "kubemem.h"
#include "unistd.h"

unsigned long parse_byte_file(char *path) {
  FILE *file = fopen(path, "r");
  if(file == NULL) {
    exit(ERR_CGROUP_READ_FAILED);
  }

  unsigned long value;
  int read_result = fscanf(file, "%lu", &value);
  if(read_result != 0 && read_result != 1) {
    exit(ERR_CGROUP_READ_FAILED);
  }

  fclose(file);
  return value;
}

double free_ram_ratio() {
  double used_bytes = 0.0;
  double bytes_limit = 0.0;
  if (access(CGROUPV2_BYTES_LIMIT, F_OK) == 0) {
    fprintf(stderr, "Cgroups V2 used.\n");
    used_bytes = (double) parse_byte_file(CGROUPV2_BYTES_USED);
    bytes_limit = (double) parse_byte_file(CGROUPV2_BYTES_LIMIT);
  } else {
    fprintf(stderr, "Cgroups V1 used.\n");
    used_bytes = (double) parse_byte_file(CGROUP_BYTES_USED);
    bytes_limit = (double) parse_byte_file(CGROUP_BYTES_LIMIT);
  }
  return used_bytes / bytes_limit;
}
