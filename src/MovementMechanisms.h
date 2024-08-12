#pragma once

// Movement Mechanisms
void MoveLeftToRightAndViceVersa(Monster* monster, Vector3 velocity) {
	if (monster->direction == MonsterMovingDirection::Right) {
		if (monster->texture->position.x <= (monster->initialPos.x + (velocity.x * 500))) {
			monster->texture->position.x += velocity.x;
			monster->texture->position.y += velocity.y;
			monster->texture->position.z += velocity.z;
		}
		else {
			monster->direction = MonsterMovingDirection::Left;
		}
	}
	else if (monster->direction == MonsterMovingDirection::Left) {
		if (monster->texture->position.x >= (monster->initialPos.x - (velocity.x * 500))) {
			monster->texture->position.x -= velocity.x;
			monster->texture->position.y -= velocity.y;
			monster->texture->position.z -= velocity.z;
		}
		else {
			monster->direction = MonsterMovingDirection::Right;
		}
	}
	monster->texture->UpdateTexture(monster->texture->position);
}

void MoveInOppositeDirections(Monster* monster, Vector3 velocity) {
	if (monster->direction == MonsterMovingDirection::Left) {
		if (monster->texture->position.x >= (monster->initialPos.x - (velocity.x * 10))) {
			monster->texture->position.x -= velocity.x;
			monster->texture->position.y -= velocity.y;
			monster->texture->position.z -= velocity.z;
		}
		else {
			monster->direction = MonsterMovingDirection::Left;
		}
	}
	else if (monster->direction == MonsterMovingDirection::Right) {
		if (monster->texture->position.x <= (monster->initialPos.x + (velocity.x * 10))) {
			monster->texture->position.x += velocity.x;
			monster->texture->position.y += velocity.y;
			monster->texture->position.z += velocity.z;
		}
		else {
			monster->direction = MonsterMovingDirection::Right;
		}
	}
	monster->texture->UpdateTexture(monster->texture->position);
}