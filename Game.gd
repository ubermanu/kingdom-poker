extends Node2D

var cards = []
var step = 0

enum Steps { NEW, BET, REVEAL, CHANGE, RESULT }

func _ready():
	$TokenList.visible = false
	cards = range(52)
	shuffle()
	var ids = cards.slice(0, 4)
	print(ids)
	$Board/Card.id = ids[0]
	$Board/Card.flipped = false

func shuffle():
	randomize()
	cards.shuffle()
