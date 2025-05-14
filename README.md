# What it does

Solves any [verbal arithmetic](https://en.wikipedia.org/wiki/Verbal_arithmetic) puzzle only involving addition.
Verbal arithmetic is also known as alphametics, cryptarithmetic, cryptarithm or word addition.

# How it works

The solver implements a simple [backtracking](https://en.wikipedia.org/wiki/Backtracking) algorithm, systematically trying letter-to-digit assignments beginning with letters of the least power and the lowest digits.

It continually extends a partial solution (a set of assignments that may be extended to a solution) by trying extensions to it, beginning with the partial solution where no letters are assigned digits (the empty set). When the numbers don't add up, it rejects an extension and backtracks. This means, it reverts the last extension and tries another one. That means, the last chosen digit didn't make sense with the prior ones and another digit is chosen if possible. Else, it backtracks again.

Eventually, this program finds all solutions (as the potential search space is rather small for a computer: Naively, there are 10! = 3,628,800 possible solutions for a problem with 10 letters. This number is reduced by backtracking cutoffs).

# Fun fact

As the program counts the solutions, theoretically, you could use it to calculate numbers of the form $10!/(10-d)!$ by counting, using the input:
```
{
  Number of inputs: 1,
           Summand: ABC...L, where L is the dth letter of the alphabet,
               Sum: [Summand]
}
```
However, this turns out to be quite slow, and only works reasonably well for $d$ up to 5 (unless you're willing to wait for an hour burning your processor. However, that would still be unreasonable).
```
...

Counter: 30238
A = 3, B = 6, C = 7, D = 8, E = 9,
ABCDE    36789
ABCDE    36789

Counter: 30239
A = 4, B = 6, C = 7, D = 8, E = 9,
ABCDE    46789
ABCDE    46789

Counter: 30240
A = 5, B = 6, C = 7, D = 8, E = 9,
ABCDE    56789
ABCDE    56789
```
