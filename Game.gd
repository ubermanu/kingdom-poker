extends Node2D

export (int) var bank = 50 setget set_bank
export (int) var pot = 0 setget set_pot
var cards = []

signal step_finished
signal bank_updated(score)
signal pot_updated(pot)

func _ready():
	$Board.visible = false
	$TokenList.visible = false
	cards = range(52)
	$TokenList/Token.connect("bet", self, "bet")
	$TokenList/Token2.connect("bet", self, "bet")
	$TokenList/Token3.connect("bet", self, "bet")
	$TokenList/Token4.connect("bet", self, "bet")
	runtime()

func set_bank(amount):
	bank = amount
	emit_signal("bank_updated", bank)

func set_pot(amount):
	pot = amount
	emit_signal("pot_updated", pot)

func bet(amount):
	if amount <= bank:
		set_bank(bank - amount)
		set_pot(pot + amount)

func runtime():
	# Reset setup
	set_bank(bank)
	set_pot(0)
	$TokenList.visible = false
	$Board.visible = false
	flip_cards(true)
	shuffle()
	update_card_ids(cards.slice(0, 4))
	
	# Show token list
	$Message.text = "Veuillez miser..."
	$TokenList.visible = true
	yield(self, "step_finished")
	
	# Select cards to swap
	$Message.text = "Changer certaines cartes?"
	$TokenList.visible = false
	$Board.visible = true
	for token in get_tree().get_nodes_in_group('Temp'):
		token.queue_free()
	flip_cards(false)
	toggle_select_cards(true)
	yield(self, "step_finished")
	
	# Resolve result code, message and bonus
	update_card_ids(cards.slice(4, 9))
	flip_cards(false)
	toggle_select_cards(false)
	var result = Combo.get_combination_code(get_card_ids())
	$Message.text = Combo.Messages[result]
	bank += pot * Combo.Bonus[result]
	
	if bank <= 0:
		$Message.text = "..."
		yield(self, "step_finished")
		get_tree().change_scene("res://Menu.tscn")
	
	yield(self, "step_finished")
	runtime()

func get_card_ids():
	return [
		$Board/Card.id,
		$Board/Card2.id,
		$Board/Card3.id,
		$Board/Card4.id,
		$Board/Card5.id
	]

# Update selected cards
func update_card_ids(ids):
	if $Board/Card.flipped:
		$Board/Card.id = ids[0]
	if $Board/Card2.flipped:
		$Board/Card2.id = ids[1]
	if $Board/Card3.flipped:
		$Board/Card3.id = ids[2]
	if $Board/Card4.flipped:
		$Board/Card4.id = ids[3]
	if $Board/Card5.flipped:
		$Board/Card5.id = ids[4]

func flip_cards(flipped):
	$Board/Card.flipped = flipped
	$Board/Card2.flipped = flipped
	$Board/Card3.flipped = flipped
	$Board/Card4.flipped = flipped
	$Board/Card5.flipped = flipped

func toggle_select_cards(selectable):
	$Board/Card.selectable = selectable
	$Board/Card2.selectable = selectable
	$Board/Card3.selectable = selectable
	$Board/Card4.selectable = selectable
	$Board/Card5.selectable = selectable

func shuffle():
	randomize()
	cards.shuffle()

func _on_NextButton_input_event(_viewport, event, _shape_idx):
	if event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT and event.pressed:
			emit_signal("step_finished")
