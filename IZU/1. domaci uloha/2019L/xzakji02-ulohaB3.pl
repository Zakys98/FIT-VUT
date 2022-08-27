
ulohaB3([], _) :- !,false.
ulohaB3(L, MAXIDT):-
	start(L, 0, 0, ID),!,
	ID = MAXIDT.

start([], IdEnd, _, IdEnd).
start(L, IdEnd, ValEnd, ID):-
	CountPom is 0,
	ValPom is 0,
	first(L, L2, IdPom, ValPom, CountPom, RetVal, RetCount),
	Prum is RetVal / RetCount,
	(ValEnd < Prum -> NewIdEnd is IdPom, NewValEnd is Prum; NewIdEnd is IdEnd, NewValEnd is ValEnd),
	start(L2, NewIdEnd, NewValEnd, ID).

first([], _, _, _, _, _, _).
first(List, P, IdPom, ValPom, CountPom, A, B):-
	member(X, List),
	mazat(List, P),
	getVal(X, Id1, Val),
	IdPom = Id1,
	incr(CountPom, CountPom2),
	add(Val, ValPom, ValPom2),
	next_member(P, X, IdPom, ValPom2, CountPom2, RetVal, RetCount),
	A is RetVal, 
	B is RetCount,
	mazat(List, P).

mazat([], _).
mazat([_|T], P):-
	append(T, [], P).

next_member([], _, _, ValPom, CountPom, ValPom, CountPom).
next_member(List, X1, IdPom, ValPom, CountPom, RetVal, RetCount):-
	member(X2, List),
	mazat(List, P),
	getVal(X1, Id1, _),
	getVal(X2, Id2, Val2),
	(Id1 = Id2 -> 
		incr(CountPom, CC),
		add(Val2, ValPom, Z),
		next_member(P, X1, IdPom, Z, CC, RetVal, RetCount)
	; next_member(P, X1, IdPom, ValPom, CountPom, RetVal, RetCount)).
	
incr(X, Y):- Y is X+1.
add(X2, Val, X):- X is X2+Val.

getVal(obj(X, Y), B, C) :-
	C is Y,
	B is X.
