%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%                 Sockets                   %%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

:-use_module(library(sockets)).

port(60070).

% launch me in sockets mode
server:-
	port(Port),
	socket_server_open(Port, Socket),
	socket_server_accept(Socket, _Client, Stream, [type(text)]),
	write('Accepted connection'), nl,
	serverLoop(Stream),
	socket_server_close(Socket).

% wait for commands
serverLoop(Stream) :-
	repeat,
	read(Stream, ClientMsg),
	write('Received: '), write(ClientMsg), nl,
	parse_input(ClientMsg, MyReply),
	format(Stream, '~q.~n', [MyReply]),
	write('Wrote: '), write(MyReply), nl,
	flush_output(Stream),
	(ClientMsg == quit; ClientMsg == end_of_file), !.

parse_input(createBoard(BoardSize, Lista), Answer) :-
	createBoard(BoardSize, Lista, Answer),!.
	
parse_input(verify_place(Linha1, Coluna1,Linha, Coluna, Player, BoardSize), Answer) :-
	verify_place(Linha1, Coluna1,Linha, Coluna, Player, BoardSize, Answer),!.
	
parse_input(replaceMatrix(Lista, Linha, Coluna,Letra,ListaC2, Bool, 0), Answer) :-
	replaceMatrix(Lista, Linha, Coluna,Letra,ListaC2, Bool, 0), write('Aro'),Answer = ListaC2,!.

parse_input(quit, ok-bye) :- !.


%%%%%%%%%%%%%%
%inicia o jogo
%%%%%%%%%%%%%%
%play(GameMode,BoardSize):-play(GameMode, BoardSize,_,_,1,1).

%play(GameMode,BoardSize,Lista, ListaTeste, Z,Player):-
%	
%	game_mode(GameMode),
%
%	((Z is 1, 
%	createBoard(BoardSize,Lista),ListaTeste = Lista, show_board(Lista),Z1 is 2, place( _, _, Lista,ListaC,ListaTeste,ListaTesteCT, Player, BoardSize) );
%	(Z is 2, Z1 is 2,
%	place( _, _, Lista,ListaC,ListaTeste,ListaTesteCT, Player, BoardSize))),
%	
%	%show_board(ListaTesteCT),
%	%artificialIntelegence(GameMode),
%	show_board(ListaC),
%	((Player is 1, Player1 is 2);
%	(Player is 2, Player1 is 1)),
%play(GameMode,BoardSize,ListaC,ListaTesteCT, Z1, Player1).
	

%%%%%%%%%%
%Game Mode
%%%%%%%%%%
game_mode('PlayerVsPlayer').
game_mode('PlayerVsComputer').
game_mode('ComputerVsComputer').

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Define qual o player e a respectiva letra do mesmo
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%player(1, 'x', 'X').
%player(2, 'y', 'Y').

%%%%%%%%%%%%%
%Fazer Jogada
%%%%%%%%%%%%%
print_player(Player):-write('Player '),write(Player), write(':'),nl.

%place(Linha, Coluna, Lista,ListaC,ListaTeste,ListaTesteCT, Player, BoardSize):-
%	print_player(Player),
%	place(Player, Linha, Coluna, BoardSize),
%	player(Player,Letra, _),
%	Linha1 is Linha, Coluna1 is Coluna,
%	replaceMatrix(Lista, Linha, Coluna,Letra,ListaC2, Bool, 0),
%	((Bool is 0,
%	place( _, _, ListaC2,ListaC,ListaTeste,ListaTesteCT, Player, BoardSize));
%	(Bool is 1,
%	L = ListaC2, verify_line(ListaC2,L,ListaC, Linha1, Coluna1, Letra, 8,ListaTeste,ListaTesteCT, BoardSize)
%	)).
	
%place(Player, Linha, Coluna, BoardSize):-
%	write('Linha = '),
%	read(Linha1),
%	Linha1 is Linha,
%	write('Coluna = '),
%	Coluna1 is Coluna,
%	read(Coluna1),!,
%	verify_place(Linha1, Coluna1, Linha, Coluna, Player, BoardSize).
	
verify_place(Linha1, Coluna1,Linha, Coluna, Player, BoardSize, Answer):-
	 (((Linha1 <1), Answer = 'Jogada invalida...Jogue outra vez!');
	 ((Linha1 > BoardSize), Answer = 'Jogada invalida...Jogue outra vez!');
	 ((Coluna1 < 1), Answer = 'Jogada invalida...Jogue outra vez!');
	 ((Coluna1 > BoardSize), Answer = 'Jogada invalida...Jogue outra vez!');
	 ((Linha1 is 1, Coluna1 is 1), Answer = 'Jogada invalida...Jogue outra vez!');
	 ((Linha1 is 1, Coluna1 == BoardSize), Answer = 'Jogada fora dos limites...Jogue outra vez!');
	 ((Linha1 == BoardSize, Coluna1 is 1), Answer = 'Jogada fora dos limites...Jogue outra vez!');
	 ((Linha1 == BoardSize, Coluna1 == BoardSize), Answer = 'Jogada fora dos limites...Jogue outra vez!');
	 ((Player is 1, Coluna1 is 1), Answer = 'Nao pode jogar fora dos limites das linhas do adversario...Jogue outra vez!');
	 ((Player is 1, Coluna1 == BoardSize), Answer = 'Nao pode jogar fora dos limites das linhas do adversario...Jogue outra vez!');
	 ((Player is 2, Linha1 is 1), Answer = 'Nao pode jogar fora dos limites das linhas do adversario...Jogue outra vez!');
	 ((Player is 2, Linha1== BoardSize), Answer = 'Nao pode jogar fora dos limites das linhas do adversario...Jogue outra vez!');
	 (Answer = 0,
	 Linha is Linha1*2,
	 Coluna is (Coluna1-1)*8+4)).
	

%%%%%%%%%%%%%%%%%
%Desenhar o board
%%%%%%%%%%%%%%%%%
show_board(Z):-show_board(Z, 0).

show_board([], _).
show_board([Z|Zs], Num):- 
	Num1 is Num+1, 
	((Num1 <20, write(' '), show(Z),nl); 
	(0 is mod(Num1,2), 
	write(' '), 
	show(Z),nl); 
	(show(Z),nl)),
show_board(Zs, Num1).

show([]).
show([L|Ls]):-
	write(L),
	show(Ls).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Altera os elementos da lista
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
replaceList([H|T], Index, X, [H|R], Bool, Bool2):- 
	Index > 0, 
	Index1 is Index-1,
	replaceList(T, Index1, X, R, Bool, Bool2).

replaceList([H|T], 0, X, [H1|T], Bool, Bool2):-
	(Bool2 is 0,
		((H \='o',
		%Answer = 'Nao pode jogar por cima de outras pecas...Jogue outra vez', nl, 
		H1 = H, Bool is 0);
		H1 = X, Bool is 1));
	(Bool2 is 1,  H1 = X);
	(Bool2 is 2, 
		(H== ' ', H1 = X);
		H1=H).

replaceMatrix([],_,_,_,[],_,_).
replaceMatrix([H|T], -1, IndexC, X, [H|Prev], Bool, Bool2):-
replaceMatrix(T, -1, IndexC, X, Prev, Bool, Bool2).
 
replaceMatrix([H|T], 0, IndexC, X, [H1|Prev], Bool, Bool2):-
replaceMatrix(T, -1, IndexC, X, Prev, Bool, Bool2),
replaceList(H, IndexC, X, H1, Bool, Bool2).

replaceMatrix([H|T],IndexR, IndexC, X, [H|R], Bool, Bool2):- 
IndexR > 0, 
IndexR1 is IndexR-1, 
replaceMatrix(T, IndexR1,IndexC, X, R, Bool, Bool2).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Constroi a linha caso seja necessário
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
possible_lines(1,4,8).
possible_lines(2,4,-8).
possible_lines(3,2,-16).
possible_lines(4,-2,-16).
possible_lines(5,-4,-8).
possible_lines(6,-4,8).
possible_lines(7,-2,16).
possible_lines(8,2,16).

verify_matrix([_|T],IndexR, IndexC, X,LetraM,Bool,Bool2):-
	IndexR > 0, 
	IndexR1 is IndexR-1, 
verify_matrix(T, IndexR1,IndexC, X,LetraM,Bool,Bool2).
 
verify_matrix([H|_], 0, IndexC, X,LetraM,Bool,Bool2):-
verify_list(H, IndexC, X,LetraM,Bool,Bool2).

verify_list([_|T], Index, X,LetraM,Bool,Bool2):- 
	Index > 0, 
	Index1 is Index-1,
	verify_list(T, Index1, X,LetraM,Bool,Bool2).
	
verify_list([H|_], 0, X,LetraM, Bool,0):-
	((H\=X,H\=LetraM, Bool is 0);%Nao pode criar
	(Bool is 1)).%pode criar

verify_list([H|_], 0, _, LetraM, Bool,1):-
	((H\='-',H\=' ',H\='|',H\=LetraM, Bool is 0);
	(Bool is 1)).

verify_list([H|_], 0, _, LetraM, Bool,2):-
	((H==LetraM, Bool is 0);
	(Bool is 1)).
	

verify_line(_,Lista2, ListaC, _,_,_, 0,ListaTeste, ListaTesteCT, _):-ListaC = Lista2, ListaTesteCT=ListaTeste.
verify_line(Lista, Lista2, ListaC, Linha, Coluna, X, Value, ListaTeste, ListaTesteCT, BoardSize):-
	Value>0, 
	%write('Value ='),write(Value), nl,
	Value1 is Value-1,
	possible_lines(Value, Linha1, Coluna1),
	Linha2 is Linha+Linha1, Coluna2 is Coluna+Coluna1,
	Linha3 is round((Linha+Linha2)/2), Coluna3 is round((Coluna+Coluna2)/2),
	player(_,X, LetraM),
	((Linha2 < (BoardSize + 1) * 2, Linha2 > 1,Coluna2 > 3, Coluna2 < 3 + (BoardSize - 1)  * 8 + 2,
		verify_matrix(Lista, Linha2, Coluna2, X,LetraM,  Bool,0),
		verify_matrix(Lista, Linha3, Coluna3, _, LetraM,  Bool1,1),
		((Bool is 1,Bool1 is 1,
			draw_temp(ListaTeste,Linha, Coluna,Linha2, Coluna2, LetraM, ListaTesteCT1),
			verify_matrix(ListaTeste, Linha3, Coluna3, _,LetraM,  Bool2,2),
			verify_matrix(ListaTesteCT1, Linha3, Coluna3, _,LetraM,  Bool3,1),
			((Bool2 is 1, Bool3 is 1, draw_line(Lista2,Linha, Coluna,Linha2, Coluna2,Linha3, Coluna3, LetraM, ListaCT));
			(Bool2 is 0, ListaCT = Lista2);(Bool3 is 0, ListaCT = Lista2)));
		(Bool is 0, ListaCT = Lista2, ListaTesteCT1=ListaTeste); (Bool1 is 0, ListaCT = Lista2, ListaTesteCT1=ListaTeste)));
	(Coluna2 < 4, ListaCT = Lista2, ListaTesteCT1=ListaTeste); (Coluna2 > 3 + (BoardSize - 1) * 8 + 1, ListaCT = Lista2, ListaTesteCT1=ListaTeste);
	(Linha2 > (BoardSize + 1) * 2 - 1, ListaCT = Lista2, ListaTesteCT1=ListaTeste); (Linha2 < 2, ListaCT = Lista2, ListaTesteCT1=ListaTeste)),
verify_line(Lista, ListaCT, ListaC, Linha, Coluna, X, Value1, ListaTesteCT1, ListaTesteCT, BoardSize).
	
draw_line(Lista, Linha, Coluna, Linha2, Coluna2,Linha3, Coluna3, LetraM, ListaCT):-
	replaceMatrix(Lista,Linha2, Coluna2, LetraM, ListaCT1, _, 1),
	replaceMatrix(ListaCT1,Linha, Coluna, LetraM, ListaCT2, _, 1),
	replaceMatrix(ListaCT2,Linha3, Coluna3, LetraM, ListaCT, _, 1).
	
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%desenha linhas temporarias so para verificar se é possivel a criaçao de novas linhas
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
draw_temp(Lista, Linha, Coluna, Linha2, Coluna2, LetraM, ListaCT):-
	((Coluna2 < Coluna, Linha2 < Linha,
		L2 is Linha2 -1,  C2 is Coluna2 - 1,
		draw_temp_line(Lista,Linha, Coluna,L2, C2, LetraM, ListaCT));
	(Coluna2 > Coluna, Linha2 > Linha,
		L1 is Linha-1,  C1 is Coluna - 1,
		draw_temp_line(Lista,Linha2, Coluna2,L1, C1, LetraM, ListaCT));
	(Coluna2 < Coluna, Linha2 > Linha,
		L1 is Linha -1,  C2 is Coluna2 - 1,
		draw_temp_line(Lista,Linha2, Coluna,L1, C2, LetraM, ListaCT));
	(Coluna2 > Coluna, Linha2 < Linha,
		L2 is Linha2 -1,  C1 is Coluna - 1,
		draw_temp_line(Lista,Linha, Coluna2,L2, C1, LetraM, ListaCT))).
	

draw_temp_line(Lista, Linha,_, Linha,_,_,ListaCT):-ListaCT=Lista.
draw_temp_line(Lista, Linha, Coluna, Linha2, Coluna2, LetraM, ListaCT):- 
	Linha1 is Linha2+1,
	cycle(Lista, Linha1, Coluna, Coluna2, LetraM, ListaCT1),
draw_temp_line(ListaCT1, Linha, Coluna,Linha1, Coluna2, LetraM,ListaCT).

cycle(Lista, _, Coluna, Coluna, _, ListaCT):-ListaCT=Lista.
cycle(Lista, Linha, Coluna, Coluna2, LetraM, ListaCT):-
	Coluna1 is Coluna2+1,
	%write('Coluna = '), write(Coluna1), write('   Linha = '), write(Linha), nl,
	replaceMatrix(Lista,Linha, Coluna1, LetraM, ListaCT1, _, 2),
cycle(ListaCT1, Linha, Coluna, Coluna1, LetraM, ListaCT).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Verifica se um jogador ganhou
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%win =1 se ganha, win=0 caso contrarios
verify_first(Lista, Player, Linha, Coluna, LetraM):- 
	((Player is 1, verify_p1(Lista, Player, 2, Coluna, LetraM), Tamanho is Linha-2, verify_p1(Lista, Player, Tamanho, Coluna, LetraM));
	(Player is 2, verify_p2(Lista, Player, Linha, 4, LetraM), Tamanho2 is Coluna-8, verify_p2(Lista, Player, Linha, Tamanho2, LetraM))).

verify_p1(_,_,16,4,_).
verify_p1(_,_,2,4,_).
verify_p1(Lista, Player, Linha, Coluna, LetraM):-
	Coluna > 3,
	Coluna1 is Coluna-8,
	verify_matrix(Lista, Linha, Coluna1, _,LetraM,  Bool,2),
	((Bool is 0,  write('Verifica'), nl);
	(Bool is 1, write('Nao verifica'), nl)),
verify_p1(Lista, Player, Linha, Coluna1, LetraM).

verify_p2(_,_,2,60,_).
verify_p2(_,_,2,4,_).
verify_p2(Lista, Player, Linha, Coluna, LetraM):-
	Linha > 1,
	Linha1 is Linha-2,
	verify_matrix(Lista, Linha1, Coluna, _,LetraM,  Bool,2),
	((Bool is 0,  write('Verifica'), nl);
	(Bool is 1, write('Nao verifica'), nl)),
verify_p2(Lista, Player, Linha1, Coluna, LetraM).
	
	
	
	
	
	
	
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%





%Insere o elemento X no fim da lista
insertEnd(X,[H|T],[H|NewT]):-insertEnd(X,T,NewT).
insertEnd(X,[],[X]).
	
%Adiciona N elementos X ao fim da lista

addElement(List, NewList, _, 0):-
	NewList = List.
	
addElement(List, NewList, X, N):-
	N1 is N-1,
	insertEnd(X, List, AuxList),
	addElement(AuxList, NewList, X, N1).
	


%Cria o header do board, deve-se chamar com Counter = 1


boardHeader(0, 0, NewList, OldList):-
	NewList = OldList.
	
boardHeader(N, Counter, NewList, OldList):-
	(Counter == 1,
		addElement([], ListAux1, ' ', 4),
		addElement(ListAux1, ListAux2, Counter, 1),
		Counter1 is Counter+1,
		boardHeader(N, Counter1, NewList, ListAux2));
	(Counter < N,
		((Counter<10, addElement(OldList, ListAux1, ' ', 7)); (addElement(OldList, ListAux1, ' ', 6))),
		addElement(ListAux1, ListAux2, Counter, 1),
		Counter1 is Counter+1,
		boardHeader(N, Counter1, NewList, ListAux2));
	(Counter == N,
		((Counter<10, addElement(OldList, ListAux1, ' ', 7)); (addElement(OldList, ListAux1, ' ', 6))),
		addElement(ListAux1, ListAux2, Counter, 1),
		boardHeader(0, 0, NewList, ListAux2)).
		
boardHeader(N, List):-
	boardHeader(N, 1, List, []).

%Adicionar uma slot e o seu espacamento, Tabuleiro NxN, Slot I

addSlot(N, 1, OldList, NewList):-
	addElement(OldList, ListAux1, ' ', 3),
	addElement(ListAux1, ListAux2, 'o', 1),
	addSlot(N, 2, ListAux2, NewList).
	
addSlot(N, I, OldList, NewList):-
	(I is N - 1,
		addElement(OldList, ListAux1, ' ', 3),
		addElement(ListAux1, ListAux2, '|', 1),
		addElement(ListAux2, ListAux3, ' ', 3),
		addElement(ListAux3, ListAux4, 'o', 1),
		NewList = ListAux4
	);
	(I < N - 1,
		addElement(OldList, ListAux1, ' ', 7),
		addElement(ListAux1, ListAux2, 'o', 1),
		I1 is I + 1,
		addSlot(N, I1, ListAux2, NewList)
	).
	
	
%Criar uma Linha do tabuleiro

createLine(I, N, NewList):-
	addElement([], ListAux1, I, 1),
	addElement(ListAux1, ListAux2, ' ', 3),
	addElement(ListAux2, ListAux3, 'o', 1),
	addElement(ListAux3, ListAux4, ' ', 3),
	addElement(ListAux4, ListAux5, '|', 1),
	addSlot(N, 1, ListAux5, ListAux6),
	NewList = ListAux6.

%Criar todas as linhas do tabuleiro, I deve comecar a 1

createLines(1, N, OldMatrix, NewMatrix):-

	addElement([], AuxList1, ' ', 4),

	addElement(AuxList1, AuxList2, '-', 4),

	addElement(AuxList2, AuxList3, '|', 1),

	addElement(AuxList3, AuxList4, '-', (8 * (N - 2)) -1),

	addElement(AuxList4, AuxList5, '|', 1),

	addElement(AuxList5, AuxList6, '-', 4),
	addElement([], GapList, ' ',  4 +(N * 8)),
	createLine(1, N, ListAux),
	addElement(OldMatrix, AuxMatrix, GapList, 1),
	addElement(AuxMatrix, AuxMatrix1, ListAux, 1),
	addElement(AuxMatrix1, NextMatrix, AuxList6, 1),
	createLines(2, N, NextMatrix, NewMatrix).
	
	
	
createLines(I, N, OldMatrix, NewMatrix):-
	(I is N,
		createLine(I, N, ListAux),
		addElement(OldMatrix, NextMatrix, ListAux, 1),
		NewMatrix = NextMatrix
	);
	(I is N - 1,
		addElement([], AuxList1, ' ', 4),
		addElement(AuxList1, AuxList2, '-', 4),
		addElement(AuxList2, AuxList3, '|', 1),
		addElement(AuxList3, AuxList4, '-', (8 * (N - 2)) -1),
		addElement(AuxList4, AuxList5, '|', 1),
		addElement(AuxList5, GapList, '-', 4),
		createLine(I, N, ListAux),
		I1 is I + 1,
		addElement(OldMatrix, AuxMatrix, ListAux, 1), % Espacos
		addElement(AuxMatrix, NextMatrix, GapList, 1), % Intervalo
		createLines(I1, N, NextMatrix, NewMatrix)
	);
	(I < N,
		addElement([], AuxList1, ' ', 8),
		addElement(AuxList1, AuxList2, '|', 1),
		addElement(AuxList2, AuxList3, ' ', (8 * (N - 2)) -1),
		addElement(AuxList3, AuxList4, '|', 1),
		addElement(AuxList4, GapList, ' ', 4),
		createLine(I, N, ListAux),
		I1 is I + 1,
		addElement(OldMatrix, AuxMatrix, ListAux, 1), % Espacos
		addElement(AuxMatrix, NextMatrix, GapList, 1), % Intervalo
		createLines(I1, N, NextMatrix, NewMatrix)
	).
		
% Cria um board N x N

createBoard(N, NewMatrix, Answer):-
	%Criar o header
	boardHeader(N, Header),
	insertEnd(Header, [], MatrixAux1),
	
	createLines(1, N, MatrixAux1, MatrixAux2),
	insertEnd(_, MatrixAux2, MatrixAux3),
	Matrix = MatrixAux3,
	NewMatrix = Matrix,
	Answer = NewMatrix.
