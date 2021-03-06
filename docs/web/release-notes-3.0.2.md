% Release notes of the 3.0.2 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2017

# Tickets fixed

## Ticket #80: Adding the possiblity to compile a law using python module

`MFront` relies on external tools, called generators, for this step
(this semantic is the same than the one used by `cmake`).

The only available generator is currently the `Makefile` generator,
handled by the `MakefileGenerator` class. This generator can be used
as follows:

~~~~{.python}
import mfront
dsl = mfront.getDSL("Chaboche.mfront")

dsl.setInterfaces(['castem'])
dsl.analyseFile("Chaboche.mfront")
dsl.generateOutputFiles()

m = mfront.MakefileGenerator()
m.exe(dsl.getTargetsDescription())
~~~~

The `exe` methods provides several overloads and may accept an
additional argument of type `GeneratorOptions` which let the user
specifiy the optimisation level to be used.

For more details, see: <https://sourceforge.net/p/tfel/tickets/80/>

## Ticket #78: Fix transpose of `st2tost2` when using rvalue reference

The definition of the transpose of `st2tost2` was broken when the
argument was an rvalue reference.

For more details, see: <https://sourceforge.net/p/tfel/tickets/78/>

## Ticket #77: operators `*=` and `/=` are broken for fourth order tensors

The operators `*=` and `/=` are broken for fourth order tensors: the
`std::is_same` metafunction return value must be accessed with the
`value` member, not the `cond` member. This was a reminiscence an old
remove class called `tfel::meta::IsSame` which was removed in
`TFEL-3.0`.

For more details, see: <https://sourceforge.net/p/tfel/tickets/77/>

## Ticket #75: Support for `python3` in the python interface

The code generated by mfront with the python interface is not
compatible with `python3`. The trouble is linked to changes in the
module declaration. See
<https://docs.python.org/3/howto/cporting.html> for details.

For more details, see: <https://sourceforge.net/p/tfel/tickets/75/>

## Ticket #72: Error at compilation of documentation

The latex documents declared a unnecessary dependency to the
`stmaryrd` package. This package became outdated in recent `LaTeX`
distributions causing some portability issues. This dependency has
been removed.

The inclusion of the `babel` package was missing in the
`docs/presentations/mfront-formation-tp/tp.tex.in` file.

For more details, see: <https://sourceforge.net/p/tfel/tickets/72/>

## Ticket #69: `@Profiling` true; Error in cygwin

Under Windows, the `__declspec(dllexport)` attribute does not
propagate to inner structures, wheres the visibility flags used under
`POSIX` systems does. This attribute is now set also on the
`BehaviourProfiler::Timer` structure.

For more details, see: <https://sourceforge.net/p/tfel/tickets/69/>

## Ticket #64: Change the library prefix to "cyg" under cygwin

The default library prefix under cygwin has been changed to `cyg`
(rather than `lib`) to follow the standard conventions of this
environment.

For more details, see: <https://sourceforge.net/p/tfel/tickets/84/>

## Ticket #63: Error cygwin (make)

Compiling `MFront` under `Cygwin` was broken because the `_GNU_SOURCE`
macro was not defined in the following headers:

- `mfront\tests\properties\test-cxx.cxx`
- `src\System\ProcessManager.cxx`
- `src\System\SignalManager.cxx`

For more details, see: <https://sourceforge.net/p/tfel/tickets/83/>

## Ticket #58: Fix initialisation of an array of parameters

For more details, see: <https://sourceforge.net/p/tfel/tickets/58/>

## Ticket #57: More robust detection of python versions: avoid mismatch between the python libraries, the python interpreter and the boost python library

For more details, see: <https://sourceforge.net/p/tfel/tickets/57/>

## Ticket #54: Bug in StensorFromTinyVectorView: standard assignement fails

For more details, see: <https://sourceforge.net/p/tfel/tickets/54/>

## Ticket #53: Add comments handling in parameters txt files

For more details, see: <https://sourceforge.net/p/tfel/tickets/53/>

## Ticket #52: More robust handling of parameters txt file

For more details, see: <https://sourceforge.net/p/tfel/tickets/52/>

<!-- Local IspellDict: english -->
