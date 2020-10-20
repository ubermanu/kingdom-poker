extends Node2D

var cards = []
var step = 0

enum Steps { NEW, BET, REVEAL, CHANGE, RESULT }

func _ready():
	$TokenList.visible = false
	cards = range(52)
	shuffle()
	update_cards()
	flip_cards(true)

func update_cards():
	var ids = cards.slice(0, 4)
	$Board/Card.id = ids[0]
	$Board/Card2.id = ids[1]
	$Board/Card3.id = ids[2]
	$Board/Card4.id = ids[3]
	$Board/Card5.id = ids[4]

func flip_cards(flipped):
	$Board/Card.flipped = flipped
	$Board/Card2.flipped = flipped
	$Board/Card3.flipped = flipped
	$Board/Card4.flipped = flipped
	$Board/Card5.flipped = flipped

func shuffle():
	randomize()
	cards.shuffle()
