import QtQuick

BlueprintItem {

	Column {
		anchors.fill: parent

		Repeater {
			model: 3

		Row {
			anchors.left: parent.left
			anchors.right: parent.right

			Text {
				text: "Address:"
			}
			Rectangle {
				color: "transparent"
				border.width: 1
				border.color: "#000000"

				TextInput {
					text: "127.0.0.1"
				}
			}
		}

		}
	}

}