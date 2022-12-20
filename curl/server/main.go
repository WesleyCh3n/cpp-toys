package main

import (
	"fmt"
	"os"

	"github.com/gofiber/fiber/v2"
	"github.com/gofiber/fiber/v2/middleware/logger"
)

var i = 0

func main() {
	app := fiber.New()
	app.Use(logger.New())

	app.Get("/", func(c *fiber.Ctx) error {
		return c.SendString("Hello, World 👋!\n")
	})

	app.Post("/", func(c *fiber.Ctx) error {
		// Get raw body from POST request:
		fmt.Println(string(c.Body()))
		return c.Send(c.Body())
	})

	app.Post("/upload", func(c *fiber.Ctx) error {
		// Get raw body from POST request:
		file, err := c.FormFile("sendfile")
		if err != nil {
			return err
		}
		i += 1
		c.SaveFile(file, fmt.Sprintf("./%d-%s", i, file.Filename))
		os.Remove(fmt.Sprintf("./%d-%s", i, file.Filename))
		// fmt.Println(string(c.Body()))
		return nil
	})

	app.Listen(":3000")
}
