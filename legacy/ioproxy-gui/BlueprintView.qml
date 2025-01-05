import QtQuick 2.15

Rectangle {
	property var bp
	id: view

	BlueprintBackground {
		anchors.fill: parent
	}

	Component {
		id: itemComp

		BlueprintItem {
			x: 30
			y: 30
		}
	}

	Component {
		id: tcpSocketComp

		BlueprintTcpSocket {
			x: 500
			y: 30
		}
	}

	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////

	QtObject {
		id: d
		property var items: []
	}

	Component.onCompleted: function() {
		// Setup items from existing.
//		for (var i = 0; i < bp.items.length; i++) {
//			var item = createQmlItem(bp.items[i])
//			d.items.push(item)
//		}

		itemComp.createObject(view)
		tcpSocketComp.createObject(view)
	}
}
