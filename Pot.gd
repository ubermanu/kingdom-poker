extends Node2D

func _on_Game_pot_updated(pot):
	$Amount.set_text(String(pot))
