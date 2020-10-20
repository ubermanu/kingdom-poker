extends Area2D

export (bool) var active = true

func _ready():
	update()

func update():
	if active:
		$Icon.frame_coords.x = 0
		$Music.play()
		$Crowd.play()
	else: 
		$Icon.frame_coords.x = 1
		$Music.stop()
		$Crowd.stop()

func toggle():
	active = !active
	update()

func _on_MusicButton_input_event(_viewport, event, _shape_idx):
	if event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT and event.pressed:
			toggle()
