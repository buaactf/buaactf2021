let extraScore = 1;

let wuDi = true;

let firstFruit = 0;

let reverseLevelUp = false;

const minRandomFruitNum = reverseLevelUp ? 6 : 0;
const maxRandomFruitNum = reverseLevelUp ? 11 : 5;
let setFruits = {
  startFruits: reverseLevelUp ? [10, 10, 9, 8, 8, 7] : [0, 0, 1, 2, 2, 3],
  randomFunction: () => {
    return minRandomFruitNum + Math.floor(Math.random() * (maxRandomFruitNum - minRandomFruitNum));
  }
}

let fruitQTan = false;

let fruitSlowDown = false;

let clickChangeFruit = true;

let adLink = '';

document.getElementsByTagName("title")[0].innerText = '合成大西瓜';

let selectModal = false;

