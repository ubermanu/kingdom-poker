extends Node2D

func _on_Game_score_updated(score):
	$Score.set_text(String(score))
