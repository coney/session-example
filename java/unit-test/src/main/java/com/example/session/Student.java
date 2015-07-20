package com.example.session;

/**
 * Created by coney on 7/20/15.
 */
public class Student {
    private FizzNumber game;
    private int id;

    public Student(FizzNumber game, int id) {
        this.game = game;
        this.id = id;
    }

    public String say() {
        if (id % game.getNumber1() == 0) {
            return "Fizz";
        } else if (id % game.getNumber2() == 0) {
            return "Buzz";
        } else if (id % game.getNumber3() == 0) {
            return "Whizz";
        } else {
            return String.valueOf(id);
        }
    }
}
