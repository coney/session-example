package com.example.session;

import org.junit.Test;

import java.util.List;

import static org.hamcrest.CoreMatchers.is;
import static org.junit.Assert.*;

/**
 * Created by coney on 7/21/15.
 */
public class FizzGameTest {
    @Test
    public void testFizzGame() throws Exception {
        FizzNumber number = new FizzNumber(3, 5, 7);
        FizzGame game = new FizzGame(number, 100);
        List<String> list = game.say();
        assertThat(list.size(), is(100));
        assertThat(list.get(2), is("Fizz"));

    }
}