Compiled Ruleset @603 
Compiled Rule 
	--atommatch:
		spec           [2, 2]
	--memmatch:
		spec           [1, 1]
		jump           [L373, [0], [], []]
	--guard:L373:
		spec           [1, 1]
		jump           [L366, [0], [], []]
	--body:L366:
		spec           [1, 10]
		commit         [null, 0]
		newmem      [1, 0, 0]
		setmemname     [1, "init"]
		loadruleset    [1, @601]
		loadruleset    [0, @602]
		newatom     [2, 1, 5_1]
		newatom     [3, 1, 'board'_1]
		newatom     [4, 1, 'null'_1]
		newatom     [5, 1, 'k_position'_1]
		alloclink   [6, 2, 0]
		alloclink   [7, 3, 0]
		alloclink   [8, 4, 0]
		alloclink   [9, 5, 0]
		unifylinks     [6, 7, 1]
		unifylinks     [8, 9, 1]
		enqueueatom    [5]
		enqueueatom    [4]
		enqueueatom    [3]
		proceed        []


Compiled Ruleset @602   
Compiled Rule 
	--atommatch:
		spec           [2, 2]
	--memmatch:
		spec           [1, 2]
		anymem      [1, 0, 0, "init"]
		stable         [1]
		commit         ["iend", 0]
		removemem      [1, 0]
		removeproxies  [1]
		movecells      [0, 1]
		removetemporaryproxies[0]
		freemem        [1]
		proceed        []

Compiled Rule 
	--atommatch:
		spec           [2, 7]
	--memmatch:
		spec           [1, 22]
		findatom    [1, 0, 'k_position'_2]
		derefatom   [5, 1, 1]
		isint          [5]
		derefatom   [3, 1, 0]
		isint          [3]
		findatom    [2, 0, 'p'_2]
		derefatom   [6, 2, 1]
		isint          [6]
		ine            [5, 6]
		isub       [17, 5, 6]
		allocatom  [18, -1_1]
		ige            [17, 18]
		isub       [15, 5, 6]
		allocatom  [16, 1_1]
		ile            [15, 16]
		derefatom   [4, 2, 0]
		isint          [4]
		ine            [3, 4]
		isub       [13, 3, 4]
		allocatom  [14, -2_1]
		ige            [13, 14]
		isub       [11, 3, 4]
		allocatom  [12, 2_1]
		ile            [11, 12]
		isub        [7, 3, 4]
		allocatom   [8, 2_1]
		imod        [9, 7, 8]
		allocatom  [10, 0_1]
		ieq            [9, 10]
		commit         ["set1", 0]
		dequeueatom    [1]
		dequeueatom    [2]
		removeatom     [1, 0, 'k_position'_2]
		removeatom     [2, 0, 'p'_2]
		dequeueatom    [6]
		removeatom     [6, 0]
		dequeueatom    [4]
		removeatom     [4, 0]
		dequeueatom    [3]
		removeatom     [3, 0]
		dequeueatom    [5]
		removeatom     [5, 0]
		copyatom   [19, 0, 6]
		copyatom   [20, 0, 4]
		newatom    [21, 0, 'k_position'_2]
		newlink        [21, 0, 20, 0, 0]
		newlink        [21, 1, 19, 0, 0]
		enqueueatom    [21]
		freeatom       [1]
		freeatom       [2]
		freeatom       [9]
		freeatom       [18]
		freeatom       [8]
		freeatom       [17]
		freeatom       [7]
		freeatom       [11]
		freeatom       [10]
		freeatom       [12]
		freeatom       [6]
		freeatom       [4]
		freeatom       [16]
		freeatom       [15]
		freeatom       [14]
		freeatom       [13]
		freeatom       [3]
		freeatom       [5]
		proceed        []

Compiled Rule 
	--atommatch:
		spec           [2, 7]
	--memmatch:
		spec           [1, 22]
		findatom    [1, 0, 'k_position'_2]
		derefatom   [5, 1, 1]
		isint          [5]
		derefatom   [3, 1, 0]
		isint          [3]
		findatom    [2, 0, 'p'_2]
		derefatom   [6, 2, 1]
		isint          [6]
		ine            [5, 6]
		isub       [17, 5, 6]
		allocatom  [18, -2_1]
		ige            [17, 18]
		isub       [15, 5, 6]
		allocatom  [16, 2_1]
		ile            [15, 16]
		isub       [11, 5, 6]
		allocatom  [12, 2_1]
		imod       [13, 11, 12]
		allocatom  [14, 0_1]
		ieq            [13, 14]
		derefatom   [4, 2, 0]
		isint          [4]
		ine            [3, 4]
		isub        [9, 3, 4]
		allocatom  [10, -1_1]
		ige            [9, 10]
		isub        [7, 3, 4]
		allocatom   [8, 1_1]
		ile            [7, 8]
		commit         ["set2", 0]
		dequeueatom    [1]
		dequeueatom    [2]
		removeatom     [1, 0, 'k_position'_2]
		removeatom     [2, 0, 'p'_2]
		dequeueatom    [6]
		removeatom     [6, 0]
		dequeueatom    [4]
		removeatom     [4, 0]
		dequeueatom    [3]
		removeatom     [3, 0]
		dequeueatom    [5]
		removeatom     [5, 0]
		copyatom   [19, 0, 6]
		copyatom   [20, 0, 4]
		newatom    [21, 0, 'k_position'_2]
		newlink        [21, 0, 20, 0, 0]
		newlink        [21, 1, 19, 0, 0]
		enqueueatom    [21]
		freeatom       [1]
		freeatom       [2]
		freeatom       [18]
		freeatom       [16]
		freeatom       [7]
		freeatom       [17]
		freeatom       [8]
		freeatom       [9]
		freeatom       [6]
		freeatom       [10]
		freeatom       [4]
		freeatom       [13]
		freeatom       [11]
		freeatom       [12]
		freeatom       [14]
		freeatom       [15]
		freeatom       [3]
		freeatom       [5]
		proceed        []


Compiled Ruleset @601    
Compiled Rule 
	--atommatch:
		spec           [2, 3]
	--memmatch:
		spec           [1, 8]
		findatom    [1, 0, 'board'_1]
		derefatom   [2, 1, 0]
		isint          [2]
		commit         ["ini0", 0]
		dequeueatom    [1]
		removeatom     [1, 0, 'board'_1]
		dequeueatom    [2]
		removeatom     [2, 0]
		copyatom    [3, 0, 2]
		copyatom    [4, 0, 2]
		copyatom    [5, 0, 2]
		newatom     [6, 0, 'fin'_2]
		newatom     [7, 0, 'board'_3]
		newlink        [6, 0, 3, 0, 0]
		newlink        [6, 1, 7, 2, 0]
		newlink        [7, 0, 4, 0, 0]
		newlink        [7, 1, 5, 0, 0]
		enqueueatom    [7]
		enqueueatom    [6]
		freeatom       [1]
		freeatom       [2]
		proceed        []

Compiled Rule 
	--atommatch:
		spec           [2, 8]
	--memmatch:
		spec           [1, 17]
		findatom    [1, 0, 'fin'_2]
		deref       [2, 1, 1, 2]
		func           [2, 'board'_3]
		derefatom   [5, 2, 0]
		isint          [5]
		allocatom   [4, 0_1]
		ine            [5, 4]
		allocatom   [6, 1_1]
		isub        [7, 5, 6]
		getfunc     [8, 7]
		allocatomindirect[9, 8]
		derefatom   [3, 2, 1]
		isint          [3]
		commit         ["ini1", 0]
		dequeueatom    [1]
		dequeueatom    [2]
		removeatom     [1, 0, 'fin'_2]
		removeatom     [2, 0, 'board'_3]
		dequeueatom    [5]
		removeatom     [5, 0]
		dequeueatom    [3]
		removeatom     [3, 0]
		copyatom   [10, 0, 9]
		copyatom   [11, 0, 5]
		copyatom   [12, 0, 3]
		copyatom   [13, 0, 3]
		newatom    [14, 0, 'fin'_2]
		newatom    [15, 0, 'board'_3]
		newatom    [16, 0, 'p'_2]
		relink         [14, 0, 1, 0, 0]
		newlink        [14, 1, 15, 2, 0]
		newlink        [15, 0, 10, 0, 0]
		newlink        [15, 1, 12, 0, 0]
		newlink        [16, 0, 11, 0, 0]
		newlink        [16, 1, 13, 0, 0]
		enqueueatom    [16]
		enqueueatom    [15]
		enqueueatom    [14]
		freeatom       [1]
		freeatom       [2]
		freeatom       [9]
		freeatom       [6]
		freeatom       [7]
		freeatom       [4]
		freeatom       [5]
		freeatom       [3]
		proceed        []

Compiled Rule 
	--atommatch:
		spec           [2, 8]
	--memmatch:
		spec           [1, 17]
		findatom    [1, 0, 'fin'_2]
		derefatom   [3, 1, 0]
		isint          [3]
		deref       [2, 1, 1, 2]
		func           [2, 'board'_3]
		derefatom   [7, 2, 1]
		isint          [7]
		allocatom   [6, 1_1]
		igt            [7, 6]
		allocatom   [8, 1_1]
		isub        [9, 7, 8]
		getfunc    [10, 9]
		allocatomindirect[11, 10]
		derefatom   [5, 2, 0]
		isint          [5]
		allocatom   [4, 0_1]
		ieq            [5, 4]
		commit         ["ini2", 0]
		dequeueatom    [1]
		dequeueatom    [2]
		removeatom     [1, 0, 'fin'_2]
		removeatom     [2, 0, 'board'_3]
		dequeueatom    [3]
		removeatom     [3, 0]
		dequeueatom    [5]
		removeatom     [5, 0]
		dequeueatom    [7]
		removeatom     [7, 0]
		copyatom   [12, 0, 3]
		copyatom   [13, 0, 3]
		copyatom   [14, 0, 11]
		newatom    [15, 0, 'fin'_2]
		newatom    [16, 0, 'board'_3]
		newlink        [15, 0, 12, 0, 0]
		newlink        [15, 1, 16, 2, 0]
		newlink        [16, 0, 13, 0, 0]
		newlink        [16, 1, 14, 0, 0]
		enqueueatom    [16]
		enqueueatom    [15]
		freeatom       [1]
		freeatom       [2]
		freeatom       [3]
		freeatom       [11]
		freeatom       [4]
		freeatom       [6]
		freeatom       [9]
		freeatom       [8]
		freeatom       [5]
		freeatom       [7]
		proceed        []

Compiled Rule 
	--atommatch:
		spec           [2, 23]
	--memmatch:
		spec           [1, 14]
		findatom    [1, 0, 'null'_1]
		deref       [2, 1, 0, 0]
		func           [2, 'k_position'_1]
		findatom    [3, 0, 'p'_2]
		deref       [5, 3, 1, 0]
		func           [5, 1_1]
		deref       [4, 3, 0, 0]
		func           [4, 1_1]
		findatom    [6, 0, 'fin'_2]
		derefatom  [10, 6, 0]
		isint          [10]
		deref       [7, 6, 1, 2]
		func           [7, 'board'_3]
		deref       [9, 7, 1, 0]
		func           [9, 1_1]
		deref       [8, 7, 0, 0]
		func           [8, 0_1]
		commit         ["set0", 0]
		dequeueatom    [1]
		dequeueatom    [2]
		dequeueatom    [3]
		dequeueatom    [6]
		dequeueatom    [7]
		removeatom     [1, 0, 'null'_1]
		removeatom     [2, 0, 'k_position'_1]
		removeatom     [3, 0, 'p'_2]
		removeatom     [6, 0, 'fin'_2]
		removeatom     [7, 0, 'board'_3]
		removeatom     [4, 0, 1_1]
		removeatom     [5, 0, 1_1]
		removeatom     [8, 0, 0_1]
		removeatom     [9, 0, 1_1]
		dequeueatom    [10]
		removeatom     [10, 0]
		newatom    [11, 0, 1_1]
		newatom    [12, 0, 1_1]
		newatom    [13, 0, 'k_position'_2]
		newlink        [11, 0, 13, 0, 0]
		newlink        [12, 0, 13, 1, 0]
		enqueueatom    [13]
		freeatom       [1]
		freeatom       [2]
		freeatom       [3]
		freeatom       [6]
		freeatom       [7]
		freeatom       [4]
		freeatom       [5]
		freeatom       [8]
		freeatom       [9]
		freeatom       [10]
		proceed        []


Inline
