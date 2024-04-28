SELECT name FROM people, directors, ratings WHERE people.id = directors.person_id AND directors.movie_id = ratings.movie_id AND ratings.rating >= 9.0;
