wordex: wordex.c functionFiles/authentication.c functionFiles/game_logic.c functionFiles/statistics.c functionFiles/leaderboard.c functionFiles/history.c
	gcc -o wordex wordex.c functionFiles/authentication.c functionFiles/game_logic.c functionFiles/statistics.c functionFiles/leaderboard.c functionFiles/history.c

bctest: wordex.c functionFiles/authentication.c functionFiles/game_logic.c functionFiles/statistics.c functionFiles/leaderboard.c functionFiles/history.c 
	gcc -DTESTING --coverage -o bctest wordex.c functionFiles/authentication.c functionFiles/game_logic.c functionFiles/statistics.c functionFiles/leaderboard.c functionFiles/history.c

clean:
	rm -f wordex bctest *.o *.gcno *.gcda *.gcov test_results.txt