require "grape"
require "open-uri"

class CalculatorAPP < Grape::API
  format :txt
  get ":a+:b" do
    uri = "http://localhost:3000/add?a=#{params[:a]}&b=#{params[:b]}"
    open uri do |http|
      http.read
    end
  end
end
