extends Node

var default = load("res://images/cursor-default.png")
var grab = load("res://images/cursor-grab.png")

func _ready():
	Input.set_custom_mouse_cursor(default)
	Input.set_custom_mouse_cursor(grab, Input.CURSOR_DRAG)
