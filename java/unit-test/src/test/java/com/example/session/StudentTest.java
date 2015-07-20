package com.example.session;

import org.junit.Test;

import static org.hamcrest.CoreMatchers.is;
import static org.junit.Assert.*;

/**
 * Created by coney on 7/20/15.
 */
public class StudentTest {

    @Test
    public void testStudent() throws Exception {
        FizzNumber game = new FizzNumber(3, 5, 7);
        assertThat(new Student(game, 1).say(), is("1"));
        assertThat(new Student(game, 3).say(), is("Fizz"));
        assertThat(new Student(game, 5).say(), is("Buzz"));
        assertThat(new Student(game, 7).say(), is("Whizz"));
    }
    @Test
    public void testStudentNew() throws Exception {
        FizzNumber game = new FizzNumber(4,5,6);
        assertThat(new Student(game, 1).say(), is("1"));
        assertThat(new Student(game, 3).say(), is("3"));
        assertThat(new Student(game, 4).say(), is("Fizz"));
    }
}