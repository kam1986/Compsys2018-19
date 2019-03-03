# API
  * You deviate a bit from the API specifications
    * You have to write the usage to stderr not stdout
    * Your file type guesses should be aligned when multiple inputs are given

# Code
  * You forget to close the file at the end
  * You still do have a lot of magic numbers; most of them are commented (which is good) but some of them aren't, such as the UTF16 BOM's. The better solution is still to #define most of these in the top of the file, and have as few numbers in the code as possible.
  * You need to check the return codes when using library functions such as fseek, to ensure they they did not fail in a weird manner.

# Theory
## GDB
  * Almost correct; you just need to add parentheses around your input at the macro definition.
  * You haven't added your own tests
  * You managed to copy the 4B test up to 3B ... making 3B test against the wrong numbers.

## Assembly
  * What you have made seems fine

# Testing
  * Missing
  * My testing:
    * ASCCI, Empty, UTF16 and most ISO work as they should
    * Many data, UTF8, and a few ISO files never terminate
    * Of those that do terminate, some data files are reported as UTF8 and vice versa

# Report
  * Your report is lacklustre ...
  * You are supposed to describe your code in general, the non-trivial parts, what you found ambiguous and how you chose to understand it, as well as include the theory part (as you have) and at the beginning briefly describe the assignment.
  * You have to focus a lot more on this part

# General info
  * A lot of the points you can get in these assignments lay in the testing and report part. It is better to only implement half the code, but have a well written report and good tests, than to focus all on the code and only write report and test in the last second.
  * If you do not focus enough on the report, I have no clue as to whether or not you understood the assignment or if you have problems you want clarified, only that you have some code. I can only encourage you to start on tests and report a lot earlier.
