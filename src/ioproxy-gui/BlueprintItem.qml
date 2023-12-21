import QtQuick
import QtQuick.Layouts

Rectangle {
	property var bpItem

	id: root
	color: "#2955ad"
	radius: 5
	width: 300
	height: 350

	ColumnLayout {
		anchors.fill: parent
		anchors.margins: 5

		// Header element.
		Rectangle {
			id: header
			color: "#FFFFFF"
			radius: 5
			Layout.fillWidth: true
			Layout.preferredHeight: headerText.height + 2 * Constants.defaultTextPadding

			Text {
				anchors.horizontalCenter: parent.horizontalCenter
				anchors.verticalCenter: parent.verticalCenter
				text: "Title"
				id: headerText
			}
		}

		// Content element.
		// e.g. Attributes, Statistics, ...
		Rectangle {
			id: contentRect
			color: "#ff9900"
			radius: 5
			Layout.fillWidth: true
			Layout.fillHeight: true

			// DEV Begin
			/*
			ColumnLayout {
				anchors.fill: parent
				id: contentLayout
				spacing: 0

				// Attributes
				Rectangle {
					Layout.fillWidth: true
					Layout.fillHeight: false
					id: attributesRect
					color: "#ff0099"

					ColumnLayout {
						anchors.fill: parent
						id: col

						Repeater {
							model: 4

							RowLayout {
								Layout.fillWidth: true
								Layout.fillHeight: false
								height: Math.max(label.height, 10)

								Text {
									Layout.fillHeight: true
									id: label
									text: "Key"
								}
								Rectangle {
									Layout.fillHeight: true
									Layout.fillWidth: true
									border.width: 1
									border.color: "#FFFFFF"
									color: "transparent"

									TextInput {
										anchors.fill: parent
										text: ""
									}
								}
							}
						}
					}
				}

				Rectangle {
					Layout.fillWidth: true
					Layout.fillHeight: true
					id: statisticsRect
					color: "#9900FF"
				}
			}*/
			// DEV End
		}

		// Header element.
		Rectangle {
			id: footer
			color: "#FFFFFF"
			radius: 5
			Layout.fillWidth: true
			Layout.preferredHeight: footerText.height + 2 * Constants.defaultTextPadding

			Text {
				anchors.horizontalCenter: parent.horizontalCenter
				anchors.verticalCenter: parent.verticalCenter
				text: "Title"
				id: footerText
			}
		}
	}
}
