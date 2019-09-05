Matrix transpose and fast transpose
===================================

first part
-----------

1. Compile and run the code provided (c and fortran version) which performs a matrix transpose and measure its performance.
2. Try to apply the trick Luca explained in his lecture 
2. Modify it in order to write an optimized version using the Fast Transpose algorithm  
3. Use different matrix sizes (1024, 2048, 4096) and play with the block size. Plot the time of execution vs block size. Does the performance gain reach a plateau? Why?

second part
-------------

4. Using perf (see this_, or here_ ), visualize cache activity for the transpose algorithm you have at disposal.
   ( use something like ``perf stat -e L1-dcache-load-misses {your command}`` )
   


.. _this: https://perf.wiki.kernel.org/index.php/Main_Page 

.. _here: http://www.brendangregg.com/perf.html

.. _rst: https://github.com/ralsina/rst-cheatsheet/blob/master/rst-cheatsheet.rst

.. _md: https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet
