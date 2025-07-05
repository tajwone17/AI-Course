// This file contains the logic for the Tic Tac Toe game, including functions to handle player moves, check for wins or draws, and reset the game.

const board = ["", "", "", "", "", "", "", "", ""];
let currentPlayer = "X";
let gameActive = true;

const statusDisplay = document.getElementById("msg");
const boxes = document.querySelectorAll(".box");

const winningConditions = [
  [0, 1, 2],
  [3, 4, 5],
  [6, 7, 8],
  [0, 3, 6],
  [1, 4, 7],
  [2, 5, 8],
  [0, 4, 8],
  [2, 4, 6],
];

/**
 * Handles player clicks on the game board cells
 *
 * @function handleBoxClick
 * @param {Event} event - The click event object
 * @description Processes a player's move when they click on a cell:
 *              1. Identifies the clicked box and its position in the board array
 *              2. Validates if the move is legal (empty cell and game is active)
 *              3. Updates the board array with the current player's symbol
 *              4. Updates the visual display of the box with the player's symbol
 *              5. Checks if the move resulted in a win or draw
 * @returns {void}
 */
function handleBoxClick(event) {
  const clickedBox = event.target;
  const boxIndex = Array.from(boxes).indexOf(clickedBox);

  if (board[boxIndex] !== "" || !gameActive) {
    return;
  }

  board[boxIndex] = currentPlayer;
  clickedBox.textContent = currentPlayer;
  checkResult();
}

/**
 * Checks if the current game state results in a win or draw
 *
 * @function checkResult
 * @description Analyzes the current state of the game board to determine if:
 *              1. There is a winning combination (3 in a row of the same symbol)
 *              2. The game is a draw (all cells filled with no winner)
 *              3. The game should continue
 *
 *              If a player has won:
 *              - Displays the winner message with the current player symbol
 *              - Sets gameActive to false to prevent further moves
 *
 *              If it's a draw:
 *              - Displays the draw message
 *              - Sets gameActive to false
 *
 *              If game continues:
 *              - Switches to the next player (X -> O or O -> X)
 * @returns {void}
 */
function checkResult() {
  let roundWon = false;

  for (let i = 0; i < winningConditions.length; i++) {
    const [a, b, c] = winningConditions[i];
    if (board[a] === "" || board[b] === "" || board[c] === "") {
      continue;
    }
    if (board[a] === board[b] && board[a] === board[c]) {
      roundWon = true;
      break;
    }
  }

  if (roundWon) {
    statusDisplay.textContent = `Player ${currentPlayer} has won!`;
    gameActive = false;
    return;
  }

  if (!board.includes("")) {
    statusDisplay.textContent = "Game ended in a draw!";
    gameActive = false;
    return;
  }

  currentPlayer = currentPlayer === "X" ? "O" : "X";
}

/**
 * Resets the game to its initial state for a new round
 *
 * @function resetGame
 * @description Prepares the game for a new round by:
 *              1. Setting gameActive flag to true to allow moves
 *              2. Resetting currentPlayer to 'X' (player always goes first)
 *              3. Clearing the board array by filling it with empty strings
 *              4. Resetting the visual elements (box text)
 *              5. Clearing any status/winner message
 *
 *              Called when the reset button is clicked, allowing players
 *              to start a fresh game without reloading the page.
 * @returns {void}
 */
function resetGame() {
  gameActive = true;
  currentPlayer = "X";
  board.fill("");
  boxes.forEach((box) => (box.textContent = ""));
  statusDisplay.textContent = "";
}

boxes.forEach((box) => box.addEventListener("click", handleBoxClick));
document.getElementById("reset-btn").addEventListener("click", resetGame);
