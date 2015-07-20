package com.example.session;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by coney on 7/20/15.
 */
public class FizzGame {
    private int count;
    private FizzNumber fizzNumber;
    public FizzGame(FizzNumber game, int count) {
        this.count = count;
        this.fizzNumber = game;
    }

    public List<String> say() {
        ArrayList<String> list = new ArrayList<String>();
        for (int i = 1; i <= count; ++i) {
            list.add(new Student(fizzNumber, i).say());
        }
        return list;
    }
}

