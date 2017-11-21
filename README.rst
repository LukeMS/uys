UYS - Useful Yet Simple C libraries collection |travisci_badge|_ |codecov_badge|_ 
===========================================================

.. |travisci_badge| image:: https://travis-ci.org/LukeMS/uys.svg?branch=master
.. _travisci_badge: https://travis-ci.org/LukeMS/uys

.. |codecov_badge| image:: https://codecov.io/gh/LukeMS/uys/branch/master/graph/badge.svg
.. _codecov_badge: https://codecov.io/gh/LukeMS/uys

A C library (or collection of small modules and other libraries) with some useful data types and functions. 
It includes some content created by me but also integrates existing libraries from external sources (why reinvent the wheel, right?).
The collection, as a whole, has the goals of being:

* Simple (to install, understand and use);
* Useful, providing some data types and functions that the standard library does not;
* Well documented (documentation should cover both original libraries and the incorporated ones);
* Well tested (tests should cover both original libraries and the incorporated ones);
* Cohesive (both original libraries and the incorporated ones should have a similar API and look and, to achieve that, modifications are made on the latter when necessary);

The 3rd party / existing libraries incorporated are listed below:

* list.c/list.h are a modified version of the linked list algorithm presented in the book "`Mastering Algorithms with C`_" (1999) by Kyle Loudon, published by O'Reilly & Associates;

.. _`Mastering Algorithms with C`: http://shop.oreilly.com/product/9781565924536.do