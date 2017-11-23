UYS - Useful Yet Simple (C libraries collection)
===========================================================
|version_badge|_ |travisci_badge|_ |codacy_badge|_  |codecov_badge|_ |doxycoverage_badge|_

.. |version_badge| image:: https://badge.fury.io/gh/LukeMS%2Fuys.svg
.. _version_badge: https://badge.fury.io/gh/LukeMS%2Fuys

.. |travisci_badge| image:: https://travis-ci.org/LukeMS/uys.svg?branch=master
.. _travisci_badge: https://travis-ci.org/LukeMS/uys

.. |codacy_badge| image:: https://api.codacy.com/project/badge/Grade/cbc2da745aa64cb5944b412eb55f613a
.. _codacy_badge: https://www.codacy.com/app/LukeMS/uys?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=LukeMS/uys&amp;utm_campaign=Badge_Grade

.. |codecov_badge| image:: https://codecov.io/gh/LukeMS/uys/branch/master/graph/badge.svg
.. _codecov_badge: https://codecov.io/gh/LukeMS/uys

.. |doxycoverage_badge| image:: https://lukems.github.io/uys/doxy-coverage.svg
.. _doxycoverage_badge: https://lukems.github.io/uys/doxy-coverage.txt

.. contents::

About
******

A collection of C libraries with some useful data types and functions, providing some frequently needed features that the standard library does not offer.

Most of the content comes from existing libraries/code, modified when deemed necessary to provide a reasonably cohesive API, good documentation and testing.

UYS goals
**********
The collection, as a whole, aims to be:

* Simple (to install, understand and use);
* Useful, providing some data types and functions that the standard library does not offer;
* Well documented;
* Well tested;
* Cohesive;
* Portable: (at least) Windows, Linuxes and Macs should be supported (relying on Continuous Integration Services to test them);

3rd party libraries
********************

* list.c/list.h are a modified version of the linked list algorithm presented in the book "`Mastering Algorithms with C`_" (1999) by Kyle Loudon, published by O'Reilly & Associates.

.. code:: c

   /*****************************************************************
     *
     * list.h
     *
     * Source: "Mastering Algorithms with C" (1999, p. 56) by Kyle Loudon,
     *         published by O'Reilly & Associates.
     *
     * This source can be freely used, adapted, and redistributed in source or
     * binary form, so long as an acknowledgment appears in derived source files.
     * The citation should list that the code comes from the  book  "Mastering
     * Algorithms with C"  by Kyle Loudon,  published by O'Reilly & Associates.
     * This code is under copyright and cannot be included in any other book,
     * publication, or  educational product  without  permission  from  O'Reilly
     * & Associates.  No warranty is attached; we cannot take responsibility for
     * errors or  fitness for use.
     *
     *****************************************************************/

* lookup3.c was created by Bob Jenkins on May 2006 and released to the Public Domain;

.. code:: c

   /*****************************************************************
     * lookup3.c, by Bob Jenkins, May 2006, Public Domain.
     *
     * These are functions for producing 32-bit hashes for hash table lookup.
     * hashword(), hashlittle(), hashlittle2(), hashbig(), mix(), and final()
     * are externally useful functions.  Routines to test the hash are included
     * if SELF_TEST is defined.  You can use this free for any purpose.  It's in
     * the public domain.  It has no warranty.
     *****************************************************************/

.. _`Mastering Algorithms with C`: http://shop.oreilly.com/product/9781565924536.do

Todo
*****

* lcov + badge;
* keep on collecting/documenting/testing libraries;
