SELECT AVG(energy) FROM songs, artists WHERE songs.artist_id = artists.id AND artists.name = "Drake";
