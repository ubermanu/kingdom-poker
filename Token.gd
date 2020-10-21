extends KinematicBody2D

export (int) var value = 1 setget set_value
export (bool) var draggable = false

var dragging = false
var offset = Vector2.ZERO

var allowed_values = [
	1, 5, 10, 20, 100
]

func set_value(v):
	value = v
	$Sprite.frame = allowed_values.find(v)

func _process(_delta):
	if draggable and dragging:
		var mouse = get_viewport().get_mouse_position()
		position = Vector2(mouse.x - offset.x, mouse.y - offset.y)

func _on_Token_input_event(_viewport, event, _shape_idx):
	if draggable and event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT and event.pressed:
			offset = get_viewport().get_mouse_position() - position
			dragging = true
		elif event.button_index == BUTTON_LEFT and !event.pressed:
			if dragging == true:
				$DropEffect.play()
			dragging = false

func _on_VisibilityNotifier2D_screen_exited():
	queue_free()

func _on_Token_mouse_entered():
	if draggable:
		Input.set_default_cursor_shape(Input.CURSOR_DRAG)

func _on_Token_mouse_exited():
	if draggable:
		Input.set_default_cursor_shape(Input.CURSOR_ARROW)
