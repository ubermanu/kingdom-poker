extends KinematicBody2D

var dragging = false
var offset = Vector2.ZERO

func _process(_delta):
	if dragging:
		var mouse = get_viewport().get_mouse_position()
		position = Vector2(mouse.x - offset.x, mouse.y - offset.y)

func _on_Token_input_event(_viewport, event, _shape_idx):
	if event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT and event.pressed:
			offset = get_viewport().get_mouse_position() - position
			dragging = true
		elif event.button_index == BUTTON_LEFT and !event.pressed:
			dragging = false

func _on_VisibilityNotifier2D_screen_exited():
	queue_free()

func _on_Token_mouse_entered():
	Input.set_default_cursor_shape(Input.CURSOR_DRAG)

func _on_Token_mouse_exited():
	Input.set_default_cursor_shape(Input.CURSOR_ARROW)
