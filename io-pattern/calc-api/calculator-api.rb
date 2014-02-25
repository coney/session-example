require "grape"

class CalculatorAPI < Grape::API
  format :txt
  get :add do
    sleep 2
    params[:a].to_i + params[:b].to_i
  end
end
