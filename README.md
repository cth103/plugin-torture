plugin-torture
==============

This is a tool to test LADSPA and LV2 plugins.

It loads a given plugin and subjects it to some test runs.
Each one is a 1024 sample buffer containing something, then
256 1024-sample buffers of silence to let the plugin recover
(and/or generate denormals!)

These are:

- silence: all 0 samples.
- impulse: initial sample of 1, followed by 0s.
- pulse: 512 1 samples, followed by 512 0 samples.
- ardour-dc-bias: buffer full of the value that Ardour uses
   if its "DC bias" denormal protection is turned on.
- flt-min: buffer full of FLT_MIN
- denormals: buffer full of 1e-38f

The flt-min and denormals tests are only run if --evil is passed
as a parameter, since they cause lots of plugins to generate
denormals (not entirely surprisingly).

Denormals
=========

One notable feature of plugin-torture is that it can ask the processor
to raise an exception when a denormal is detected, and hence allow
debugging of such problems.  For this to work, the plugin must be compiled
with SSE support (pass -msse and -mfpmath=sse to GCC).


Command-line options
====================

-e|--evil: run particularly evil tests in addition to nicer ones.

-d|--denormals: set the CPU to raise SIGFPE on encountering a denormal.  This will
                be caught and reported.

-a|--abort: abort on SIGFPE; useful if you are gdb-ing a plugin and you want
            to see where a denormal is being introduced.

-s|--ladspa: the plugin is a LADSPA

-i|--index: index of plugin in LADSPA .so (defaults to 0)

-l|--lv2: the plugin is a LV2

-p|--plugin <plugin>: specify the plugin.  This should be a .so file for LADSPA
            	      or a .ttl file for LV2.  For LV2s the plugin must be on
                      the LV2 search path.

-g|--profile <profile>: file to specify the plugin control inputs to use; see below


Input profiles
==============

By default, plugin-torture will set the plugin's controls to their default values.
If you want to torture a plugin using non-default control values, you can specify
these values in an input profile file.  This is a text file, where each line is either

    <input-number> range <from> <step> <to>

or

    <input-number> values <value> <value> ...

In each case the input number is the index of the control input to set.  "range" sets
the input to each of a range of values, and "values" specifies precise values to use.
For example,

    4 range 0 1 42

will set input 4 to each value from 0 to 42 in steps of 1 (0, 1, 2, 3 ..., 42) and
torture the plugin each time.

    9 values 6 7 10 22

will torture the plugin with input index 9 set to the values 6, 7, 10 and 22.


Contact
=======

Contact cth@carlh.net.


Bugs
====

Yes.  The Makefile has no dependency information, so be careful if you
modify a header and rebuild (doing a make clean is probably a good idea).


Acknowledgements
================

Some of this code was inspired by / copied from Ardour and
`demolition', a LADSPA torture tester
(http://devel.tlrmx.org/audio/demolition/).  Thanks!
