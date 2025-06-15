# DOBBLE GENERATOR  

## Notes  

Tested 3 methods;

- Iterative does not work for n > 3
- Checking All Permutations works, but is too computationally expensive for n > 4
- FillUp (first fill some cards, then do the rest with permutations) works, but is too computationally expensive for n > 5

=> FillUp is the most promising, but we need a better way to fill up the remaining permutations


### Fillup strategy

1. Generate base card [ 0 1 2 3 ... n ]  
2. Generate remaining n * (n-1) cards with one symbol each  
	n-1 cards with a 0, n cards with a 1, ...  
3. Fill up the remaining symbols using permutation


Bsp n=4:

base: [ 0 1 2 3 ]  

3 additional cards for each symbol:
[0 x x x ]  
[0 x x x ]  
[0 x x x ]  
[1 x x x ]  
...
[3 x x x ]  
    
Fill 0-cards:
base: [ 0 1 2 3 ]  
[0 4 5 6 ]  
[0 7 8 9 ]  
[0 a b c ]  
[1 x x x ]  
...
[3 x x x ]  

Fill Rest with permutations.
