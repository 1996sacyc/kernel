# kernel
A kernel module with 3 sysfs entries (swap_string, calc, sum_tree)
The module handle 2 parameter : 
  1. mask: a bitmask to decide whether a sysfs entries sill be appeared or not
  2. name[1-3] : char typem, to customize the entry's name, and it will be the default name if you don't pass the parameter
