/*
*  Project     Nintendo Extension Controller Library
*  @author     David Madison
*  @link       github.com/dmadison/NintendoExtensionCtrl
*  @license    LGPLv3 - Copyright (c) 2021 David Madison
*
*  This file is part of the Nintendo Extension Controller Library.
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "NXC_LinkedList.h"

namespace NintendoExtensionCtrl {

void ExtensionList::add(Node* node) {
	if (head == nullptr) {
		head = node;
	}
	else {
		Node* last = head;
		while (true) {
			if (last->next == nullptr) break;  // found last entry
			last = last->next;
		}
		last->next = node;
	}
}

void ExtensionList::remove(Node* node) {
	if (node == head) {
		head = node->next; // Set head to next, and we're done
		return;
	}

	// Otherwise we're somewhere else in the list. Iterate through to find it.
	Node* ptr = head;

	while (ptr != nullptr) {
		if (ptr->next == node) {  // FOUND!
			ptr->next = node->next;  // Set the previous "next" as this entry's "next" (skip this object)
			break;  // Stop searching
		}
		ptr = ptr->next;  // Not found. Next entry...
	}
}

ExtensionList::Node* ExtensionList::getHead() const {
	return head;
}


ExtensionList::Node::Node(ExtensionList& l, ExtensionController& ctrl)
	: controller(ctrl), list(l)
{
	list.add(this);
}

ExtensionList::Node::~Node()
{
	list.remove(this);
}

ExtensionController& ExtensionList::Node::getController() const {
	return controller;
}

ExtensionList::Node* ExtensionList::Node::getNext() const {
	return next;
}

}  // End "NintendoExtensionCtrl" namespace
