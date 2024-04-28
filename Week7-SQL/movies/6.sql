SELECT AVG(rating) FROM ratings, movies WHERE ratings.movie_id = movies.id AND movies.year = 2012;
