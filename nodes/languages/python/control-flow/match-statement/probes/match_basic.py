command = ["go", "north"]
match command:
    case ["go", direction]:
        print(direction)
    case _:
        print("no match")
