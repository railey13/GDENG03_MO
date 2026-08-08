#pragma once
#include <unordered_map>
#include <stack>
#include <functional>
#include <iostream>

#include "Command.h"

class CommandInvoker {
public:
	void bindCommand(int key, std::function<Command*()> command) {
		commands[key] = command;
	}

	void executeCommand(int key) {
		// check if it exists
		if (commands.count(key)) {
			std::unique_ptr<Command> cmd(commands[key]()); // make a new command
			cmd->execute();
			undoStack.push(std::move(cmd));

			// clear redo stack once a new command was executed
			while (!redoStack.empty()) {
				redoStack.pop();
			}
		}
	}

	void undo() {
		if (!undoStack.empty()) {
			auto cmd = std::move(undoStack.top());
			undoStack.pop();
			cmd->undo();
			redoStack.push(std::move(cmd));

			Debug::Log("UNDO");
		}
	}

	void redo() {
		if (!redoStack.empty()) {
			auto cmd = std::move(redoStack.top());
			redoStack.pop();
			cmd->execute();
			undoStack.push(std::move(cmd));

			Debug::Log("REDO");
		}
	}

private:
	std::unordered_map<int, std::function<Command*()>> commands;

	std::stack<std::unique_ptr<Command>> undoStack;
	std::stack<std::unique_ptr<Command>> redoStack;
};

