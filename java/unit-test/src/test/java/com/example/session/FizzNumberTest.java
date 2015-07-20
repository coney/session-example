package com.example.session;

import org.junit.Test;

import static org.hamcrest.CoreMatchers.is;
import static org.junit.Assert.*;

/**
 * Created by coney on 7/20/15.
 */
public class FizzNumberTest {

    @Test
    public void testFizzGame() throws Exception {
        FizzNumber game = new FizzNumber(1, 2, 3);
        assertThat(game.getNumber1(), is(1));
        assertThat(game.getNumber2(), is(2));
        assertThat(game.getNumber3(), is(3));
    }
}