# kernel
A kernel module with 3 sysfs entries :
  1. swap_string
    * swap the string by the specified index
    * The input will be a given positive number nwith a string
  2. calc
    * simple calculator with 5 operators
    * only for integer
  3. sum_tree
    * input a tree
    * output sums form root to the leaves
  

The module handle 2 parameter : 
  1. mask: 
    * a bitmask to decide whether a sysfs entries sill be appeared or not
  2. name[1-3] : 
    * char typem
    * to customize the entry's name
    * it will be the default name if you don't pass the parameter
