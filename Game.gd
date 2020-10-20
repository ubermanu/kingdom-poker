extends Node2D

export (int) var score = 50
var cards = []
var step = 0

enum Steps { NEW, BET, REVEAL, CHANGE, RESULT }

signal score_updated(score)

func _ready():
	$TokenList.visible = false
	cards = range(52)
	emit_signal("score_updated", score)
	play_step()

func play_step():
	
	if step == 0:
		$TokenList.visible = false
		$Board.visible = false
		shuffle()
		update_cards()
		next_step()
		emit_signal("score_updated", score)
	
	if step == 1:
		$TokenList.visible = true
	
	if step == 2:
		$TokenList.visible = false
		$Board.visible = true
		flip_cards(true)


func next_step():
	step += 1
	step %= Steps.size()

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
