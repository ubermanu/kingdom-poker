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
	set_pot(pot)
	$TokenList.visible = false
	$Board.visible = false
	flip_cards(true)
	shuffle()
	update_cards()
	
	# Show token list
	$Message.text = "Veuillez miser..."
	$TokenList.visible = true
	$TokenList/Token.draggable = true
	$TokenList/Token2.draggable = true
	$TokenList/Token3.draggable = true
	$TokenList/Token4.draggable = true
	yield(self, "step_finished")
	
	# Select cards to swap
	$Message.text = "Changer certaines cartes?"
	$TokenList.visible = false
	$Board.visible = true
	flip_cards(false)
	yield(self, "step_finished")
	
	# Result
	# TODO: Generate result according to card combos
	flip_cards(false)
	$Message.text = "Perdu, on retente sa chance?"
	
	if bank <= 0:
		# TODO: End game
		pass
	
	yield(self, "step_finished")
	runtime()

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

func _on_NextButton_input_event(_viewport, event, _shape_idx):
	if event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT and event.pressed:
			emit_signal("step_finished")
