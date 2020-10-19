extends Area2D

export (bool) var active = true

func _ready():
	update_icon()

func update_icon():
	if active:
		$Icon.frame_coords.x = 0
	else: 
		$Icon.frame_coords.x = 1

func toggle():
	active = !active
	update_icon()

func _on_MusicButton_input_event(_viewport, event, _shape_idx):
	if event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT and event.pressed:
			toggle()
