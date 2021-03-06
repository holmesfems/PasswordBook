Our need: different passwords for different domain, save password infomation, but others cannot get passwords from this information
## 约定
* **PWDN**: global password remembered by user (N: 0,1,...)
* **P.BOOK**: PasswordBook that save passwords of different domain for user
* **Gen()**: Password Algorithm/Function/Generator
* **Hints()**: Tags/Info/Tips of domain, generally meaningful. (i.e. Hints(domain) => domain.
* **PWD.OUTPUT**: Password output for domain

## Encryption Plan
### Plan A
Use PWD0 to open P.BOOK. P.BOOK save passwords generated by random function

```
save:
P.BOOK(domain) := Gen()
load:
PWD0 -> P.BOOK  (use PWD0 to open P.BOOK)
PWD.OUTPUT := P.BOOK(domain)

```

pros:

* PWD.OUTPUT =/> Gen().  (If we know one password of the domain, we still cannot know others)

cons:

* Hard to remember PWD.OUTPUT
* Must use P.BOOK to get password
* PWD0/P.BOOK => All Passwords  (If we leak our PWD0, all passwords will be leaked)

### Plan B
Use PWD0 to open P.BOOK. P.BOOK save *hints* or *seed* of the domain, which is not necessary.

PWD.OUTPUT is calculated by specific algorithm

```
save:
P.BOOK(domain) := Hints(domain)   (Hints(domain) may also be suitable for human to remember)
load:
PWD0 -> P.BOOK
PWD.OUTPUT := Gen(PWD1, P.BOOK(domain))
```

example:
```
let Hints(domain) := domain
get PWD.OUTPUT = Gen(PWD1, P.BOOK(domain)) = Gen(PWD1, Hints(domain)) = Gen(PWD1, domain)
= ~domain X PWD1
```

pros:

* P.BOOK is not such necessary ( If we remember Hints(domain) and Gen )
* PWD0/P.BOOK =/> PWD.OUTPUT ( If we only know PWD0/P.BOOK we cannot know PWD.OUTPUT)

cons:

* Gen() is hard to design ( otherwise PWD.OUTPUT => P.BOOK )
* If we know one of PWD.OUTPUT and P.BOOK, we may get PWD1 then all other PWD.OUTPUT

### Plan C
```
save:
PWD.OUTPUT := GenExtra()
P.BOOK(domain) := Hints(PWD1, PWD.OUTPUT)
load:
PWD0 -> P.BOOK
PWD.OUTPUT := Gen(PWD1, P.BOOK(domain))
So: Gen() is inverse function of Hints() (They are related)
```

Difference between B and C:
* Hints() of B is Hints(domain), but in C is Hints(PWD1, PWD.OUTPUT). They use different param
* Hints() is related to Gen in C.
* PWD.OUTPUT use Gen() by B. PWD.OUTPUT is predfined by GenExtra() in C.
* User can set Hints(domain) in B, user can remember password indirectly (remember Hints and Gen). User can remember password directly in C.
