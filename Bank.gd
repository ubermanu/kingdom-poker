extends Node2D

func _on_Game_bank_updated(bank):
	$Score.set_text(String(bank))
